using System;
using System.Collections.Generic;
using System.Globalization;
using System.Threading;
using System.Threading.Tasks;

using Windows.Devices.AllJoyn;
using Windows.Networking;
using Windows.Networking.Connectivity;


using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Repository;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Configurations;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.DeviceSchema;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Models;

using RemoteMonitoring.Devices;
using RemoteMonitoring.Devices.Factory;
using RemoteMonitoring.Transport.Factory;
using RemoteMonitoring.Telemetry.Factory;
using RemoteMonitoring.Logging;

using RemoteMonitoring.Serialization;


using SensorClient.Devices.ZWaveMultisensor;
using SensorClient.DataModel;
using SensorClient.Common;

namespace SensorClient.Devices
{
    public class MultisensorDeviceFactory : IDeviceFactory
    {

        protected class Location
        {
            public string Title { get; set; }
            public double Latitude { get; set; }
            public double Longitude { get; set; }

            public Location(string title, double latitude, double longitude)
            {
                Latitude = latitude;
                Longitude = longitude;
                Title = title;
            }


        }


        // change this to inject a different logger
        private readonly ILogger _logger;
        private readonly ITransportFactory _transportFactory;
        private readonly IConfigurationProvider _configProvider;
        // private readonly WeatherShieldTelemetryFactory _telemetryFactory;
        private readonly IVirtualDeviceStorage _deviceConfiguration;


        internal MultisensorDeviceFactory(ILogger logger, IConfigurationProvider configProvider) {
        }

        public IDevice CreateDevice(ILogger logger, ITransportFactory transportFactory, 
            ITelemetryFactory telemetryFactory, IConfigurationProvider configurationProvider, InitialDeviceConfig config)
        {
            throw new NotImplementedException();
        }
    }
}
