using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Configurations;

using RemoteMonitoring;
using RemoteMonitoring.Logging;
using RemoteMonitoring.Devices;

using SensorClient.Devices.ZWaveSensor;
using SensorClient.Devices.ZWaveSensor.Telemetry;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.DeviceSchema;
using AdapterLib;

using Newtonsoft.Json;

namespace SensorClient.Devices
{
    public class ZWaveDeviceManager : DeviceManager
    {

        private static ZWaveAdapter zWaveAdapter = new ZWaveAdapter();

        public static ZWaveAdapter ZwaveAdapter { get { return zWaveAdapter; } }

        MultisensorDeviceFactory deviceFactory = null;
        private readonly ILogger _logger;
        public Dictionary<string, dynamic> devicesInitQueue { get; private set; }

        public List<IDevice> Devices { get; private set; }

        public ZWaveDeviceManager(IConfigurationProvider configProvider, ILogger logger, CancellationToken token) : base(logger, token)
        {

            this.deviceFactory = new MultisensorDeviceFactory(logger, configProvider);
            this._logger = logger;

            this.Devices = new List<IDevice>();
            this.devicesInitQueue = new Dictionary<string, dynamic>();

            zWaveAdapter.Initialize();

            zWaveAdapter.ZWaveNotification += ZWaveAdapter_ZWaveNotification;


        }

        public void SetDefaultConfiguration()
        {
            foreach (Multisensor sensor in Devices) {
                zWaveAdapter.SetDefaultConfiguration(sensor.homeID, sensor.nodeID);
            }
        }

        private void ZWaveAdapter_ZWaveNotification(ZWNotification notification)
        {
            byte nodeId = notification.GetNodeId();
            uint installationId = notification.GetHomeId();
            AdapterLib.NotificationType notificationType = notification.GetType();
            ZWValueID valueID = notification.GetValueID();

       /*    string serializedValue = JsonConvert.SerializeObject(notification, new ZWaveNotificationJsonConverter());
            this._logger.LogInfo(serializedValue);
           

            this._logger.LogInfo("Zwave notification nodeId: {0} HomeId: {1} NotificationType: {2} Genre: {3} Type: {4}  ", new object[] { nodeId, installationId, notificationType.ToString(), valueID.Genre, valueID.Type});
            if (valueID.Value != null)
            this._logger.LogInfo("notigication Value -- ValueLabel: {0} ValueHelp: {1} ValueUnits: {2} Value: {3}", new object[] {  valueID.ValueLabel, valueID.ValueHelp, valueID.ValueUnits, valueID.Value });
            */

            switch (notificationType)
            {
                case AdapterLib.NotificationType.NodeAdded:
                    {
                        // if this node was in zwcfg*.xml, this is the first node notification
                        // if not, the NodeNew notification should already have been received                        
                        dynamic node = FindInitQueueDevice(nodeId, installationId);
                        if (node == null)
                        {
                            AddDeviceToInitQueue(nodeId, installationId);
                        }

                        break;
                    }
                case AdapterLib.NotificationType.NodeNew:
                    {
                        AddDeviceToInitQueue(nodeId, installationId);
                        break;
                    }
                case AdapterLib.NotificationType.AwakeNodesQueried:
                    {
                        InitializeDevices();
                        break;
                    }

              //  case AdapterLib.NotificationType.ValueAdded:


            }
        }
        
        private void AddDeviceToInitQueue(byte nodeId, uint installationId)
        {
            // Add the new node to our list (and flag as uninitialized)
            dynamic node = this.deviceFactory.CreateZWaveDevice(nodeId, installationId);
            string deviceID = DeviceSchemaHelper.GetDeviceID(node);

            this.devicesInitQueue.Add(deviceID, node);
            this._logger.LogInfo("Added new ZWave device nodeId: {0} HomeId: {1}", new object[] { nodeId, installationId });
        }

        private async void InitializeDevices()
        {
            foreach(dynamic node in devicesInitQueue.Values)
            {
                byte nodeId = 0;
                uint installationId = 0;
                try
                {
                    
                    MultisensorDeviceFactory.parseDeviceID(node, out nodeId, out installationId);
                    ///Read node information from OpenZWave
                    NodeInfo nodeInfo = zWaveAdapter.GetNodeInfo(installationId, nodeId);

                    dynamic deviceProperties = DeviceSchemaHelper.GetDeviceProperties(node);
                    deviceProperties.Manufacturer = nodeInfo.Manufacturer;
                    deviceProperties.Platform = nodeInfo.Type;
                    deviceProperties.ModelNumber = nodeInfo.Product;

                    IZWaveDevice zWaveDevice;
                    if (nodeInfo.Type.Contains("Controller"))
                    {
                        zWaveDevice = await this.deviceFactory.CreateControllerDevice(node) as IZWaveDevice;
                    }
                    else
                    {
                        zWaveDevice = await this.deviceFactory.CreateMultisensorDevice(node) as IZWaveDevice;
                    }

                    zWaveDevice.homeID = installationId;
                    zWaveDevice.nodeID = nodeId;
                    this.Devices.Add(zWaveDevice);
                }catch(Exception ex)
                {
                    this._logger.LogInfo("Error {2} creating nodeId: {0} HomeId: {1}", new object[] { nodeId, installationId, ex.Message });
                }
            }

            devicesInitQueue.Clear();
            await StartDevicesAsync(this.Devices);
        }

        /// <summary>
        /// Try to find existing device based on device identity
        /// </summary>
        /// <param name="nodeId"></param>
        /// <param name="installationId"></param>
        /// <returns></returns>
        private dynamic FindInitQueueDevice(byte nodeId, uint installationId)
        {
            string deviceId = MultisensorDeviceFactory.MakeZWaveDeviceId(nodeId, installationId);
            if (devicesInitQueue.ContainsKey(deviceId))
                return devicesInitQueue[deviceId];
            else
                return null;
        }

      
    }
}
