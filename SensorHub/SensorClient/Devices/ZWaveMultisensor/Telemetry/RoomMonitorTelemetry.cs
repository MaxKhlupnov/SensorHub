using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RemoteMonitoring.Telemetry;
using RemoteMonitoring.Logging;
using RemoteMonitoring.Devices;
using System.Threading;


namespace SensorClient.Devices.ZWaveMultisensor.Telemetry
{
    /// <summary>
    /// Read Remote Monitor Telemetry from database and send to the cloud
    /// </summary>
    public class RoomMonitorTelemetry : ITelemetry
    {
        private readonly ILogger _logger;
        private readonly string _deviceId;

        private const int REPORT_FREQUENCY_IN_SECONDS = 5;
        private const int PEAK_FREQUENCY_IN_SECONDS = 90;

        public bool TelemetryActive { get; set; }
        public bool ActivatePresence { get; set; }

        public RoomMonitorTelemetry(ILogger logger, string deviceId)
        {
            _logger = logger;
            _deviceId = deviceId;

            ActivatePresence = false;
            TelemetryActive = true;

            int peakFrequencyInTicks = Convert.ToInt32(Math.Ceiling((double)PEAK_FREQUENCY_IN_SECONDS / REPORT_FREQUENCY_IN_SECONDS));

            // TODO: Add database connection info
        }

        public async Task SendEventsAsync(CancellationToken token, Func<object, Task> sendMessageAsync)
        {
            var monitorData = new RoomMonitorTelemetryData();
            string messageBody;
            while (!token.IsCancellationRequested)
            {
                if (TelemetryActive)
                {
                    /*   monitorData.DeviceId = _deviceId;
                       monitorData.Temperature = _temperatureReader.GetNextValue();
                       monitorData.Humidity = _humidityReader.GetNextValue();
                       messageBody = "Temperature: " + Math.Round(monitorData.Temperature, 2)
                           + " Humidity: " + Math.Round(monitorData.Humidity, 2);

                       if (ActivatePresence)
                       {
                           monitorData.Presence = _externalTemperatureGenerator.GetNextValue();
                           messageBody += " External Temperature: " + Math.Round((double)monitorData.ExternalTemperature, 2);
                       }
                       else
                       {
                           monitorData.ExternalTemperature = null;
                       }

                       //_logger.LogInfo("Sending " + messageBody + " for Device: " + _deviceId);

                       await sendMessageAsync(monitorData);*/
                }
                await Task.Delay(TimeSpan.FromSeconds(REPORT_FREQUENCY_IN_SECONDS), token);
            }
        }
    }
}
