using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Globalization;
using Windows.System.Threading;


using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.DeviceSchema;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Configurations;

using SensorClient.Devices;
using SensorClient.Devices.WeatherShield;

using RemoteMonitoring.Logging;
using RemoteMonitoring.Devices;
using RemoteMonitoring;


namespace SensorClient.DataModel
{
    public class WeatherShieldViewModel
    {
        private readonly int _devicePollIntervalSeconds;
        private const int DEFAULT_DEVICE_POLL_INTERVAL_SECONDS = 60;//120;

        private static WeatherStationConsumer weatherStationConsumer = new WeatherStationConsumer();
        private ILogger logger;

        AllJoynDeviceFactory deviceFactory = null;
        public List<WeatherShieldDevice> Devices { get; private set; }


        private static Mutex mutex = new Mutex(false, "temporaryUIMutex");

        public WeatherShieldViewModel(ILogger logger, IConfigurationProvider configProvider)
        {

           this.logger = logger;
           this.deviceFactory = new AllJoynDeviceFactory(logger, configProvider);

           this.Devices = new List<WeatherShieldDevice>();
         

            weatherStationConsumer.HumiditySensorSessionStarted += SensorStarted;
            weatherStationConsumer.TemperatureSensorSessionStarted += SensorStarted;
            weatherStationConsumer.PerssureSensorSessionStarted += SensorStarted;

            string pollingIntervalString = configProvider.GetConfigurationSettingValueOrDefault(
                                        "DevicePollIntervalSeconds",
                                        DEFAULT_DEVICE_POLL_INTERVAL_SECONDS.ToString(CultureInfo.InvariantCulture));

            _devicePollIntervalSeconds = Convert.ToInt32(pollingIntervalString, CultureInfo.InvariantCulture);
        }




        public async void SensorStarted(AbstractSensor sensor, dynamic device)
        {
            if (device == null)
                return;

            bool hasMutex = false;

            try
            {

                hasMutex = mutex.WaitOne(1000);
                if (hasMutex)
                {
                    string deviceID = DeviceSchemaHelper.GetDeviceID(device);
                    WeatherShieldDevice currentDevice = this.FindDevice(deviceID);

                    if (currentDevice == null)
                    {/// This is new device - create one and add them in the list

                        currentDevice = await this.deviceFactory.CreateWeatherShieldDevice(device);

                        this.Devices.Add(currentDevice);
                    }
                    currentDevice.AddDeviceSensor(sensor);
                }
            }
            finally
            {
                if (hasMutex)
                {
                    mutex.ReleaseMutex();
                }
            }
        }

        private WeatherShieldDevice FindDevice(string deviceID)
        {
            return this.Devices.FirstOrDefault<WeatherShieldDevice>(dev => dev.DeviceID.Equals(deviceID, StringComparison.OrdinalIgnoreCase));
        }


        /// <summary>
        /// Retrieves a set of device configs from the repository and creates devices with this information
        /// Once the devices are built, they are started
        /// </summary>
        /// <param name="token"></param>
        public async Task ProcessDevicesAsync(CancellationToken token)
        {
                var dm = new DeviceManager(logger, token);
                while (!token.IsCancellationRequested)//!cancellationTokenSource.Token.IsCancellationRequested
                {

                    logger.LogInfo("Sending devices telemery starts..");
                    try
                    {
                        
                        IEnumerable<IDevice> devicesToProcess = this.Devices.Cast<IDevice>();
                        if (devicesToProcess.Count<IDevice>() > 0)
                        {
#pragma warning disable 4014
                            //don't wait for this to finish
                            await dm.StartDevicesAsync(new List<IDevice>(devicesToProcess));
#pragma warning restore 4014
                        }

                        await Task.Delay(TimeSpan.FromSeconds(_devicePollIntervalSeconds), token);
                    }
                    catch (TaskCanceledException) {
                        //do nothing if task was cancelled
                        logger.LogInfo("********** Primary worker role cancellation token source has been cancelled. **********");
                    }
                    finally
                    {
                        //ensure that all devices have been stopped
                        dm.StopAllDevices();
                    }
        }

        }

    }
}
