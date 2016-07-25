using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RemoteMonitoring.Telemetry.Factory;
using RemoteMonitoring.Logging;
using RemoteMonitoring.Devices;

using SensorClient.Devices.ZWaveSensor.Telemetry;


namespace SensorClient.Devices.ZWaveSensor
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
           
            var roomTelemetry = new ZWaveSensorTelemetry(_logger, device.DeviceID);
            device.TelemetryEvents.Add(roomTelemetry);
           


            return roomTelemetry;
        }
    }
}
