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


using RemoteMonitoring.Logging;
using RemoteMonitoring.Devices;
using RemoteMonitoring;

namespace SensorClient.DataModel
{
    public class MultisensorViewModel
    {
        private readonly int _devicePollIntervalSeconds;
        private const int DEFAULT_DEVICE_POLL_INTERVAL_SECONDS = 60;//120;

        
        private ILogger logger;


        private static Mutex mutex = new Mutex(false, "temporaryUIMutex");

        public MultisensorViewModel(ILogger logger, IConfigurationProvider configProvider)
        {

            this.logger = logger;
          
            /*  weatherStationConsumer.HumiditySensorSessionStarted += SensorStarted;
             weatherStationConsumer.TemperatureSensorSessionStarted += SensorStarted;
             weatherStationConsumer.PerssureSensorSessionStarted += SensorStarted;*/

            string pollingIntervalString = configProvider.GetConfigurationSettingValueOrDefault(
                                        "DevicePollIntervalSeconds",
                                        DEFAULT_DEVICE_POLL_INTERVAL_SECONDS.ToString(CultureInfo.InvariantCulture));

            _devicePollIntervalSeconds = Convert.ToInt32(pollingIntervalString, CultureInfo.InvariantCulture);
        }

  

        /// <summary>
        /// Retrieves a set of device configs from the repository and creates devices with this information
        /// Once the devices are built, they are started
        /// </summary>
        /// <param name="token"></param>
        public async Task ProcessDevicesAsync(CancellationToken token)
        {
            var dm = new DeviceManager(logger, token);
        }
        }
}
