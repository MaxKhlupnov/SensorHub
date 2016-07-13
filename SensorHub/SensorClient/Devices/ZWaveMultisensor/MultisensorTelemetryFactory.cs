using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RemoteMonitoring.Telemetry.Factory;
using RemoteMonitoring.Logging;
using RemoteMonitoring.Devices;

using SensorClient.Devices.ZWaveMultisensor.Telemetry;


namespace SensorClient.Devices.ZWaveMultisensor
{
    public class MultisensorTelemetryFactory : ITelemetryFactory
    {
        private readonly ILogger _logger;

        public MultisensorTelemetryFactory(ILogger logger)
        {
            _logger = logger;
        }

        public object PopulateDeviceWithTelemetryEvents(IDevice device)
        {
            var startupTelemetry = new StartupTelemetry(_logger, device);
            device.TelemetryEvents.Add(startupTelemetry);
           
            var roomTelemetry = new RoomMonitorTelemetry(_logger, device.DeviceID);
            device.TelemetryEvents.Add(roomTelemetry);

            var presenceTelemetry = new PresenceTelemetry(_logger, device.DeviceID);
            device.TelemetryEvents.Add(presenceTelemetry);


            return roomTelemetry;
        }
    }
}
