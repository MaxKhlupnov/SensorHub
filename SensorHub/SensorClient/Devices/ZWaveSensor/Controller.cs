using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RemoteMonitoring.Devices;
using RemoteMonitoring.Logging;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Configurations;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.DeviceSchema;
using RemoteMonitoring.Transport.Factory;
using RemoteMonitoring.Telemetry.Factory;
using RemoteMonitoring.CommandProcessors;

using SensorClient.Devices.ZWaveSensor.Telemetry;
using SensorClient.Devices.ZWaveSensor.CommandProcessors;

namespace SensorClient.Devices.ZWaveSensor
{
    public class Controller : DeviceBase, IZWaveDevice
    {

        public byte nodeID { get; set; }
        public uint homeID { get; set; }

        public Controller(ILogger logger, ITransportFactory transportFactory, ITelemetryFactory telemetryFactory,
        IConfigurationProvider configurationProvider) : base(logger, transportFactory, telemetryFactory, configurationProvider)
        {

        }

        /// <summary>
        /// Builds up the set of commands that are supported by this device
        /// </summary>
        protected override void InitCommandProcessors()
        {
            var pingDeviceProcessor = new PingDeviceProcessor(this);
            var startCommandProcessor = new StartCommandProcessor(this);
            var stopCommandProcessor = new StopCommandProcessor(this);
            
            var changeDeviceStateCommmandProcessor = new ChangeDeviceStateCommandProcessor(this);

            pingDeviceProcessor.NextCommandProcessor = startCommandProcessor;
            startCommandProcessor.NextCommandProcessor = stopCommandProcessor;
            stopCommandProcessor.NextCommandProcessor = changeDeviceStateCommmandProcessor;

            RootCommandProcessor = pingDeviceProcessor;
        }
    }
}
