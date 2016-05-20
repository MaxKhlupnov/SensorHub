using System;
using System.Collections.Generic;
using System.Globalization;
using System.Threading;
using System.Threading.Tasks;

using Windows.Devices.AllJoyn;

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


using SensorClient.Devices.WeatherShield;
using SensorClient.DataModel;
using SensorClient.Common;

namespace SensorClient.Devices
{
    public class AllJoynDeviceFactory : IDeviceFactory
    {
        private const string OBJECT_TYPE_DEVICE_INFO = "AllJoynDevice";

        private const string VERSION_1_0 = "1.0";

        private const int MAX_COMMANDS_SUPPORTED = 6;

        private const bool IS_SIMULATED_DEVICE = false;

        // change this to inject a different logger
        private readonly ILogger _logger;
        private readonly ITransportFactory _transportFactory;
        private readonly IConfigurationProvider _configProvider;
        private readonly WeatherShieldTelemetryFactory _telemetryFactory;
        private readonly IVirtualDeviceStorage _deviceConfiguration;

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

        private static List<Location> _possibleDeviceLocations = new List<Location>{
            new Location("Microsoft Red West Campus, Building A", 47.659159, -122.141515),  // 
            new Location("800 Occidental Ave S, Seattle, WA 98134",47.593307, -122.332165),  // 
            new Location("11111 NE 8th St, Bellevue, WA 98004", 47.617025, -122.191285),  // 
            new Location("3003 160th Ave SE Bellevue, WA 98008", 47.583582, -122.130622)  // 
        };

        internal AllJoynDeviceFactory(ILogger logger, IConfigurationProvider configProvider)
        {

            this._logger = logger;
            this._configProvider = configProvider;
            this. _deviceConfiguration = new DeviceConfigTableStorage(_configProvider);

            this._telemetryFactory = new WeatherShieldTelemetryFactory(_logger);
            var serializer = new JsonSerialize();
            this._transportFactory = new IotHubTransportFactory(serializer, _logger, _configProvider);
        }
        public async Task<WeatherShieldDevice> CreateWeatherShieldDevice(dynamic device)
        {
            string deviceID = DeviceSchemaHelper.GetDeviceID(device);
            InitialDeviceConfig config = await this._deviceConfiguration.GetDeviceAsync(deviceID);
            if (config == null)
            {
                config = new InitialDeviceConfig();
                ///HostName=MtcDataCenter.azure-devices.net;DeviceId=makhluDev;SharedAccessKey=Q3e1wSyrkpspcR06m11bNw==
                config.DeviceId = deviceID;
                config.Key = @"EKW9OBSzAOEQ3rXrBJMSPQ==";///@"UpnqqiFPnndWF5HeiXcIOQ=="//string.Empty;
                config.HostName = @"mtcdatacenter.azure-devices.net";//MtcDataCenter.azure-devices.netstring.Empty;                        
            }
            WeatherShieldDevice newDevice =  this.CreateDevice(this._logger, this._transportFactory, this._telemetryFactory,
                this._configProvider, config) as WeatherShieldDevice;
            newDevice.DeviceProperties = DeviceSchemaHelper.GetDeviceProperties(device);
            newDevice.Init(config);
            return newDevice;
        }

        public IDevice CreateDevice(ILogger logger, ITransportFactory transportFactory,
            ITelemetryFactory telemetryFactory, IConfigurationProvider configurationProvider, InitialDeviceConfig config)
        {
            var device = new WeatherShieldDevice(logger, transportFactory, telemetryFactory, configurationProvider);
            // device.Init(config);
            return device;
        }

        public static dynamic CreateAllJoynDevice(AllJoynAboutDataView deviceDataView)
        {
            string DeviceId = deviceDataView.DeviceName;

            ///Make shre we don't have breakets in the guid
            Guid DeviceIdGuid = Guid.Empty;
            if (Guid.TryParse(DeviceId, out DeviceIdGuid)){
                DeviceId = DeviceIdGuid.ToString("D");
            }
            ;
            var device = DeviceSchemaHelper.BuildDeviceStructure(DeviceId, IS_SIMULATED_DEVICE,"IOT_HUB_ID");

            AssignDeviceProperties(deviceDataView, device);
            AssignCommands(device);
            device.ObjectType = OBJECT_TYPE_DEVICE_INFO;
            device.Version = VERSION_1_0;
            device.IsSimulatedDevice = IS_SIMULATED_DEVICE;
          
           // 

            return device;
        }

        public static dynamic CreateDummyDevice(string deviceId)
        {
            var device = DeviceSchemaHelper.BuildDeviceStructure(deviceId, true, "IOT_HUB_ID");
            dynamic deviceProperties = DeviceSchemaHelper.GetDeviceProperties(device);

            deviceProperties.HubEnabledState = false;
            deviceProperties.Manufacturer = "makhlu";
            deviceProperties.ModelNumber = "0.0";
            deviceProperties.SerialNumber = "0.0.0";
            deviceProperties.Platform = "test";

            AssignCommands(device);
            device.ObjectType = OBJECT_TYPE_DEVICE_INFO;
            device.Version = VERSION_1_0;
            device.IsSimulatedDevice = true;

            return device;
        }

        private static void AssignDeviceProperties(AllJoynAboutDataView deviceDataView, dynamic device)
        {
            dynamic deviceProperties = DeviceSchemaHelper.GetDeviceProperties(device);
            
            deviceProperties.HubEnabledState = true;
            deviceProperties.Manufacturer = deviceDataView.Manufacturer;
            deviceProperties.ModelNumber = deviceDataView.ModelNumber;
            deviceProperties.SerialNumber = deviceDataView.DeviceId;
            deviceProperties.Platform = deviceDataView.HardwareVersion;

            ulong version = 0;
            if (!ulong.TryParse(deviceDataView.SoftwareVersion, out version))
            {
                deviceProperties.FirmwareVersion = String.Empty;
            }
            else
            {
                deviceProperties.FirmwareVersion = String.Format(CultureInfo.InvariantCulture, "{0}.{1}.{2}.{3}",
                            (version & 0xFFFF000000000000) >> 48,
                            (version & 0x0000FFFF00000000) >> 32,
                            (version & 0x00000000FFFF0000) >> 16,
                            version & 0x000000000000FFFF);
            }

            /*deviceProperties.Processor = "ARM";
            deviceProperties.InstalledRAM = "No data";

            // Choose a location between the 3 above and set Lat and Long for device properties
            int chosenLocation = GetIntBasedOnString(deviceDataView.DeviceId + "Location", _possibleDeviceLocations.Count);
            deviceProperties.Latitude = _possibleDeviceLocations[chosenLocation].Latitude;
            deviceProperties.Longitude = _possibleDeviceLocations[chosenLocation].Longitude;*/
        }

        private static int GetIntBasedOnString(string input, int maxValueExclusive)
        {
            int hash = input.GetHashCode();

            //Keep the result positive
            if(hash < 0)
            {
                hash = -hash;
            }

            return hash % maxValueExclusive;
        }

        private static void AssignCommands(dynamic device)
        {
            dynamic command = CommandSchemaHelper.CreateNewCommand("PingDevice");
            CommandSchemaHelper.AddCommandToDevice(device, command);
            
            command = CommandSchemaHelper.CreateNewCommand("StartTelemetry");
            CommandSchemaHelper.AddCommandToDevice(device, command);
            
            command = CommandSchemaHelper.CreateNewCommand("StopTelemetry");
            CommandSchemaHelper.AddCommandToDevice(device, command);
            
            command = CommandSchemaHelper.CreateNewCommand("ChangeSetPointTemp");
            CommandSchemaHelper.DefineNewParameterOnCommand(command, "SetPointTemp", "double");
            CommandSchemaHelper.AddCommandToDevice(device, command);
            
            command = CommandSchemaHelper.CreateNewCommand("DiagnosticTelemetry");
            CommandSchemaHelper.DefineNewParameterOnCommand(command, "Active", "boolean");
            CommandSchemaHelper.AddCommandToDevice(device, command);
            
            command = CommandSchemaHelper.CreateNewCommand("ChangeDeviceState");
            CommandSchemaHelper.DefineNewParameterOnCommand(command, "DeviceState", "string");
            CommandSchemaHelper.AddCommandToDevice(device, command);
        }

      /*  private static void AssignTelemetry(dynamic device, AbstractSensor sensor)
        {
            dynamic telemetry = CommandSchemaHelper.CreateNewTelemetry(sensor.Title, sensor.Title, "double");
            CommandSchemaHelper.AddTelemetryToDevice(device, telemetry);

        }*/

    }
}
