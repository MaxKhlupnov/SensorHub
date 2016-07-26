using System;
using System.Collections.Generic;
using System.Globalization;
using System.Threading;
using System.Threading.Tasks;

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
using AdapterLib;


using SensorClient.Devices.ZWaveSensor;
using SensorClient.DataModel;
using SensorClient.Common;

namespace SensorClient.Devices.ZWaveSensor
{
    public class MultisensorDeviceFactory : IDeviceFactory
    {

        private const string OBJECT_TYPE_DEVICE_INFO = "ZWaveMultisensor";

        private const string VERSION_1_0 = "1.0";

        private const int MAX_COMMANDS_SUPPORTED = 6;

        private const bool IS_SIMULATED_DEVICE = false;

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
        private readonly MultisensorTelemetryFactory _telemetryFactory;
        private readonly IVirtualDeviceStorage _deviceConfiguration;


        internal MultisensorDeviceFactory(ILogger logger, IConfigurationProvider configProvider) {
            this._logger = logger;
            this._configProvider = configProvider;
            this._deviceConfiguration = new DeviceConfigTableStorage(_configProvider);

            var serializer = new JsonSerialize();
            this._transportFactory = new IotHubTransportFactory(serializer, _logger, _configProvider);
            this._telemetryFactory = new MultisensorTelemetryFactory(_logger);

        }
        public IDevice CreateDevice(ILogger logger, ITransportFactory transportFactory, ITelemetryFactory telemetryFactory,
           IConfigurationProvider configurationProvider, InitialDeviceConfig config)
        {
            var device = new Multisensor(logger, transportFactory, telemetryFactory, configurationProvider);           
            return device;
        }

        public async Task<Multisensor> CreateMultisensorDevice(dynamic device)
        {
            string deviceID = DeviceSchemaHelper.GetDeviceID(device);
            
            // TODO: read device configuration from config database
            InitialDeviceConfig config = await this._deviceConfiguration.GetDeviceAsync(deviceID);
            if (config == null)
            {
                config = new InitialDeviceConfig();
                ///HostName=MtcDataCenter.azure-devices.net;DeviceId=makhluDev;SharedAccessKey=Q3e1wSyrkpspcR06m11bNw==
                config.DeviceId = deviceID;
                config.Key = @"2OJ5+VP9z0il1FLMljagnA==";///SQowcaxvaxxE+ZSo4V/lEA==
                config.HostName = @"mtcdatacenter.azure-devices.net";//MtcDataCenter.azure-devices.netstring.Empty;                        
            }

            Multisensor newDevice = this.CreateDevice(this._logger, this._transportFactory, this._telemetryFactory,
               this._configProvider, config) as Multisensor;
            newDevice.DeviceProperties = DeviceSchemaHelper.GetDeviceProperties(device);
            newDevice.Init(config);

            return newDevice;
        }


        public async Task<Controller> CreateControllerDevice(dynamic device)
        {
            string deviceID = DeviceSchemaHelper.GetDeviceID(device);

            // TODO: read device configuration from config database
            InitialDeviceConfig config = await this._deviceConfiguration.GetDeviceAsync(deviceID);
            if (config == null)
            {
                config = new InitialDeviceConfig();
                ///HostName=MtcDataCenter.azure-devices.net;DeviceId=makhluDev;SharedAccessKey=Q3e1wSyrkpspcR06m11bNw==
                config.DeviceId = deviceID;
                config.Key = @"3shWH7GvccSI0pUWsUdPSQ==";///SQowcaxvaxxE+ZSo4V/lEA==
                config.HostName = @"mtcdatacenter.azure-devices.net";//MtcDataCenter.azure-devices.netstring.Empty;                        
            }

            Controller newDevice = new Controller(this._logger, this._transportFactory, this._telemetryFactory, this._configProvider);
            newDevice.DeviceProperties = DeviceSchemaHelper.GetDeviceProperties(device);
            newDevice.Init(config);

            return newDevice;
        }


        public dynamic CreateZWaveDevice(byte nodeId, uint installationId)
        {
            string DeviceId = MakeZWaveDeviceId(nodeId, installationId);


            var device = DeviceSchemaHelper.BuildDeviceStructure(DeviceId, IS_SIMULATED_DEVICE, "IOT_HUB_ID");

           // AssignDeviceProperties(deviceDataView, device);
           // AssignCommands(device);
            device.ObjectType = OBJECT_TYPE_DEVICE_INFO;
            device.Version = VERSION_1_0;
            device.IsSimulatedDevice = IS_SIMULATED_DEVICE;

            // 

            return device;
        }



        private static void AssignDeviceProperties(ZWNotification nodeNotificationData, dynamic device)
        {
            dynamic deviceProperties = DeviceSchemaHelper.GetDeviceProperties(device);

         /*   deviceProperties.HubEnabledState = true;
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
            */
            /*deviceProperties.Processor = "ARM";
            deviceProperties.InstalledRAM = "No data";

            // Choose a location between the 3 above and set Lat and Long for device properties
            int chosenLocation = GetIntBasedOnString(deviceDataView.DeviceId + "Location", _possibleDeviceLocations.Count);
            deviceProperties.Latitude = _possibleDeviceLocations[chosenLocation].Latitude;
            deviceProperties.Longitude = _possibleDeviceLocations[chosenLocation].Longitude;*/
        }

        /// <summary>
        /// Creates unique deivice id based on ZWave identification
        /// </summary>
        /// <param name="nodeId"></param>
        /// <param name="installationId"></param>
        /// <returns></returns>
        internal static string MakeZWaveDeviceId(byte nodeId, uint installationId)
        {
            return nodeId + "_" + installationId;
        }

        internal static void parseDeviceID(dynamic node, out byte nodeID, out uint installationId)
        {
            string deviceID = DeviceSchemaHelper.GetDeviceID(node);
            string[] ids = deviceID.Split('_');
            byte.TryParse(ids[0], out nodeID);
            uint.TryParse(ids[1], out installationId);
        }


    }
}
