using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RemoteMonitoring.Telemetry;
using RemoteMonitoring.Logging;
using RemoteMonitoring.Devices;
using System.Threading;

using AdapterLib;

namespace SensorClient.Devices.ZWaveSensor.Telemetry
{
    public class ZWaveNotificationTelemetry : ITelemetry
    {
        public bool TelemetryActive { get; set; }
        /// <summary>
        /// Type of notification we capture
        /// </summary>
        public AdapterLib.NotificationType NotificationType { get; set; }
        public ZWaveNotificationTelemetry(ILogger logger, AdapterLib.NotificationType notificationType)
        {
            TelemetryActive = true;
            NotificationType = notificationType;
        }

        public Task SendEventsAsync(CancellationToken token, Func<object, Task> sendMessageAsync)
        {
            throw new NotImplementedException();
        }
    }
}
