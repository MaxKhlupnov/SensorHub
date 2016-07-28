
using RemoteMonitoring.Logging;

using Windows.Foundation.Diagnostics;
using Windows.Storage;
using System.Threading.Tasks;
using System;

namespace SensorClient.Common
{
    /// <summary>
    /// Default implementation of ILogger with the System.Diagnostics.Trace 
    /// object as the logger.
    /// </summary>
    public class TraceLogger : ILogger, IDisposable
    {


        #region LoggingScenario constants and privates.

                private const string Prefix = "SensorHub_";
                public const string DEFAULT_SESSION_NAME = Prefix + "Session";
                public const string DEFAULT_CHANNEL_NAME = Prefix + "Info";                


        /// <summary>
        /// TraceLogger moves generated logs files into the 
        /// this folder under the LocalState folder.
        /// </summary>
        public const string TraceLogger_LOG_FILE_FOLDER_NAME = Prefix + "LogFiles";

        /// <summary>
        /// We'll keep one 256 Mb archive file
        /// </summary>
        string ARCHIVE_LOG_FILE_NAME = "SensorHub_Log_Archive.etl";

        /// <summary>
        /// The TraceLogger session.
        /// </summary>

        private FileLoggingSession session;


        /// <summary>
        /// The channel.
        /// </summary>
        private LoggingChannel channel;

        /// <summary>
        /// The number of times LogFileGeneratedHandler has been called.
        /// </summary>
        public int LogFileGeneratedCount { get; private set; }


        #endregion


        public TraceLogger()
        {
            LogFileGeneratedCount = 0;
            channel = new LoggingChannel(DEFAULT_CHANNEL_NAME);

            //   channel.LoggingEnabled += Channel_LoggingEnabled;
            App.Current.Suspending += OnAppSuspending;
            App.Current.Resuming += OnAppResuming;


            // If the app is being launched (not resumed), the 
            // following call will activate logging if it had been
            // activated during the last suspend. 
            ResumeLoggingIfApplicable();

            StartLogging();
        }

        private void OnAppResuming(object sender, object e)
        {
            // If logging was active at the last suspend,
            // ResumeLoggingIfApplicable will re-activate 
            // logging.
            ResumeLoggingIfApplicable();
        }

        private async void OnAppSuspending(object sender, Windows.ApplicationModel.SuspendingEventArgs e)
        {
            // Get a deferral before performing any async operations
            // to avoid suspension prior to LoggingScenario completing 
            // PrepareToSuspendAsync().
            var deferral = e.SuspendingOperation.GetDeferral();
            
            // Prepare logging for suspension.
            await PrepareToSuspendAsync();

            // From LoggingScenario's perspective, it's now okay to 
            // suspend, so release the deferral. 
            deferral.Complete();
        }

        private void StartLogging()

        {

            CheckDisposed();
            // If no session exists, create one.            
            if (session == null)
            {
                session = new FileLoggingSession(DEFAULT_SESSION_NAME);
                session.LogFileGenerated += LogFileGeneratedHandler;

            }

            session.AddLoggingChannel(channel, LoggingLevel.Error);
            session.AddLoggingChannel(channel, LoggingLevel.Warning);
            session.AddLoggingChannel(channel, LoggingLevel.Information);

        }

        ~TraceLogger()
        {
            Dispose(false);
        }

        /// This handler is called by the FileLoggingSession instance when a log
        /// file reaches a size of 256MB. When FileLoggingSession calls this handler, 
        /// it's effectively giving the developer a chance to take ownership of the
        /// log file. Typically, this handler should move or rename the log file.
        /// Note that once this handler has returned, the FileLoggingSession is free
        /// to reuse the original log file name for a new log file at any time.
        /// </summary>
        /// <param name="sender">The FileLoggingSession which has generated a new file.</param>
        /// <param name="args">Contains a StorageFile field LogFileGeneratedEventArgs.File representing the new log file.</param>

        private async void LogFileGeneratedHandler(IFileLoggingSession sender, LogFileGeneratedEventArgs args)
        {
            LogFileGeneratedCount++;
            StorageFolder sampleAppDefinedLogFolder =

                await ApplicationData.Current.LocalFolder.CreateFolderAsync(TraceLogger_LOG_FILE_FOLDER_NAME,
                    CreationCollisionOption.OpenIfExists);

            await args.File.MoveAsync(sampleAppDefinedLogFolder, ARCHIVE_LOG_FILE_NAME, NameCollisionOption.ReplaceExisting);
        }
      

        public void LogInfo(string message)
        {
            channel.LogMessage(message, LoggingLevel.Information);
        }

        public void LogInfo(string format, params object[] args)
        {
            string message = String.Format(format, args);
            LogInfo(message);
        }

        public void LogWarning(string message)
        {
            channel.LogMessage(message, LoggingLevel.Warning);
        }

        public void LogWarning(string format, params object[] args)
        {
            string message = String.Format(format, args);
            LogWarning(message);
        }

        public void LogError(string message)
        {
            channel.LogMessage(message, LoggingLevel.Error);
        }

        public void LogError(string format, params object[] args)
        {
            string message = String.Format(format, args);
            LogError(message);
        }

        #region Scenario code for suspend/resume.

        private const string LOGGING_ENABLED_SETTING_KEY_NAME = Prefix + "LoggingEnabled";
        private const string LOGFILEGEN_BEFORE_SUSPEND_SETTING_KEY_NAME = Prefix + "LogFileGeneratedBeforeSuspend";


        public bool IsLoggingEnabled
        {

            get
            {
                return session != null;
            }
        }
        public bool IsPreparingForSuspend { get; private set; }

        /// <summary>
        /// Prepare this scenario for suspend. 
        /// </summary>
        /// <returns></returns>
        public async Task PrepareToSuspendAsync()
        {
            CheckDisposed();

            if (session != null)
            {
                IsPreparingForSuspend = true;

                try
                {
                    // Before suspend, save any final log file.
                    string finalFileBeforeSuspend = await CloseSessionSaveFinalLogFile();
                    session.Dispose();
                    session = null;
                    // Save values used when the app is resumed or started later.
                    // Logging is enabled.
                    ApplicationData.Current.LocalSettings.Values[LOGGING_ENABLED_SETTING_KEY_NAME] = true;
                    // Save the log file name saved at suspend so the sample UI can be 
                    // updated on resume with that information. 
                    ApplicationData.Current.LocalSettings.Values[LOGFILEGEN_BEFORE_SUSPEND_SETTING_KEY_NAME] = finalFileBeforeSuspend;
                }
                finally
                {
                    IsPreparingForSuspend = false;
                }
            }
            else
            {
                // Save values used when the app is resumed or started later.
                // Logging is not enabled and no log file was saved.
                ApplicationData.Current.LocalSettings.Values[LOGGING_ENABLED_SETTING_KEY_NAME] = false;
                ApplicationData.Current.LocalSettings.Values[LOGFILEGEN_BEFORE_SUSPEND_SETTING_KEY_NAME] = null;
            }
        }


        private async Task<string> CloseSessionSaveFinalLogFile()
        {
            // Save the final log file before closing the session.
            StorageFile finalFileBeforeSuspend = await session.CloseAndSaveToFileAsync();
            if (finalFileBeforeSuspend != null)
            {
                return finalFileBeforeSuspend.Path;
            }
            else
            {
                return null;
            }
        }


        /// <summary>
        /// This is called when the app is either resuming or starting. 
        /// It will enable logging if the app has never been started before
        /// or if logging had been enabled the last time the app was running.
        /// </summary>
        public void ResumeLoggingIfApplicable()
        {
            CheckDisposed();

            object loggingEnabled;
            if (ApplicationData.Current.LocalSettings.Values.TryGetValue(LOGGING_ENABLED_SETTING_KEY_NAME, out loggingEnabled) == false)
            {
                ApplicationData.Current.LocalSettings.Values[LOGGING_ENABLED_SETTING_KEY_NAME] = true;
                loggingEnabled = ApplicationData.Current.LocalSettings.Values[LOGGING_ENABLED_SETTING_KEY_NAME];
            }

            if (loggingEnabled is bool && (bool)loggingEnabled == true)
            {
                StartLogging();
            }

            // When the sample suspends, it retains state as to whether or not it had
            // generated a new log file at the last suspension. This allows any
            // UI to be updated on resume to reflect that fact. 
            object LogFileGeneratedBeforeSuspendObject;
            if (ApplicationData.Current.LocalSettings.Values.TryGetValue(LOGFILEGEN_BEFORE_SUSPEND_SETTING_KEY_NAME, out LogFileGeneratedBeforeSuspendObject) &&
                LogFileGeneratedBeforeSuspendObject != null &&
                LogFileGeneratedBeforeSuspendObject is string)
            {               
                ApplicationData.Current.LocalSettings.Values[LOGFILEGEN_BEFORE_SUSPEND_SETTING_KEY_NAME] = null;
            }
        }

        #endregion

        #region IDisposable handling


        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);

        }



        protected virtual void Dispose(bool disposing)
        {
            if (isDisposed == false)
            {
                isDisposed = true;
                if (disposing)
                {
                    if (channel != null)
                    {
                        channel.Dispose();
                        channel = null;
                    }
                    if (session != null)
                    {
                        session.CloseAndSaveToFileAsync();
                        session.Dispose();
                        session = null;
                    }
                }
            }
        }



        /// <summary>
        /// Set to 'true' if Dispose() has been called.
        /// </summary>

        private bool isDisposed = false;

        /// <summary>
        /// Helper function for other methods to call to check Dispose() state.
        /// </summary>

        private void CheckDisposed()
        {
            if (isDisposed)
            {
                throw new ObjectDisposedException("LoggingScenario");
            }
        }
        #endregion
    }
}
