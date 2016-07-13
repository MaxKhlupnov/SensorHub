using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Configurations;

using RemoteMonitoring;
using RemoteMonitoring.Logging;

using SensorClient.Devices.ZWaveMultisensor;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.DeviceSchema;
using AdapterLib;

namespace SensorClient.Devices
{
    public class ZWaveDeviceManager : DeviceManager
    {

        private static ZWaveAdapter zWaveAdapter = new ZWaveAdapter();

        MultisensorDeviceFactory deviceFactory = null;
        private readonly ILogger _logger;
        public Dictionary<string, dynamic> devicesInitQueue { get; private set; }

        public List<Multisensor> Devices { get; private set; }

        public ZWaveDeviceManager(IConfigurationProvider configProvider, ILogger logger, CancellationToken token) : base(logger, token)
        {

            this.deviceFactory = new MultisensorDeviceFactory(logger, configProvider);
            this._logger = logger;

            this.Devices = new List<Multisensor>();
            this.devicesInitQueue = new Dictionary<string, dynamic>();

            zWaveAdapter.Initialize();

            zWaveAdapter.ZWaveNotification += ZWaveAdapter_ZWaveNotification;


        }

        private void ZWaveAdapter_ZWaveNotification(ZWNotification notification)
        {
            byte nodeId = notification.GetNodeId();
            uint installationId = notification.GetHomeId();
            AdapterLib.NotificationType notificationType = notification.GetType();

            this._logger.LogInfo("Zwave otification nodeId: {0} HomeId: {1} NotificationType {2}", new object[] { nodeId, installationId, notificationType.ToString() });


            switch (notificationType)
            {
                case AdapterLib.NotificationType.NodeAdded:
                    {
                        // if this node was in zwcfg*.xml, this is the first node notification
                        // if not, the NodeNew notification should already have been received                        
                        dynamic node = FindInitQueueDevice(nodeId, installationId);
                        if (node == null)
                        {
                            node = this.deviceFactory.CreateZWaveDevice(nodeId, installationId);                           
                        }

                        break;
                    }
                case AdapterLib.NotificationType.NodeNew:
                    {
                        // Add the new node to our list (and flag as uninitialized)
                        dynamic node = this.deviceFactory.CreateZWaveDevice(nodeId, installationId);
                        string deviceID = DeviceSchemaHelper.GetDeviceID(node);

                        this.devicesInitQueue.Add(deviceID, node);
                        break;
                    }
                case AdapterLib.NotificationType.AwakeNodesQueried:
                    {
                        InitializeDevices();
                        break;
                    }

            }
        }
        

        private async void InitializeDevices()
        {
            foreach(dynamic node in devicesInitQueue.Values)
            {
                byte nodeId = 0;
                uint installationId = 0;
                MultisensorDeviceFactory.parseDeviceID(node, out nodeId, out installationId);
                ///Read node information from OpenZWave
                NodeInfo nodeInfo = zWaveAdapter.GetNodeInfo(installationId, nodeId);

                dynamic deviceProperties = DeviceSchemaHelper.GetDeviceProperties(node);
                deviceProperties.Manufacturer = nodeInfo.Manufacturer;
                deviceProperties.Platform = nodeInfo.Type;

            //    Multisensor roomSensor = await this.deviceFactory.CreateMultisensorDevice(node);
           //     this.Devices.Add(roomSensor);
            }

            devicesInitQueue.Clear();
        }

        /// <summary>
        /// Try to find existing device based on device identity
        /// </summary>
        /// <param name="nodeId"></param>
        /// <param name="installationId"></param>
        /// <returns></returns>
        private dynamic FindInitQueueDevice(byte nodeId, uint installationId)
        {
            string deviceId = MultisensorDeviceFactory.MakeMultisensorDeviceId(nodeId, installationId);
            return devicesInitQueue[deviceId];
        }

      
    }
}
