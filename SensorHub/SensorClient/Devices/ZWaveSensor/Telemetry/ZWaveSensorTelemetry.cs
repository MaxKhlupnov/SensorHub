using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RemoteMonitoring.Telemetry;
using RemoteMonitoring.Logging;
using RemoteMonitoring.Devices;
using System.Threading;
using SensorClient.Devices;

using Newtonsoft.Json;

using AdapterLib;

namespace SensorClient.Devices.ZWaveSensor.Telemetry
{
    /// <summary>
    /// Read Remote Monitor Telemetry from database and send to the cloud
    /// </summary>
    public class ZWaveSensorTelemetry : ITelemetry
    {
        private readonly ILogger _logger;
        private readonly string _deviceId;

        private const int REPORT_FREQUENCY_IN_SECONDS = 5;
        private const int PEAK_FREQUENCY_IN_SECONDS = 90;

        public bool TelemetryActive { get; set; }
        public bool ActivatePresence { get; set; }

        private Queue<ZWaveSensorTelemetryData> TelemetryQueue = new Queue<ZWaveSensorTelemetryData>();

        public ZWaveSensorTelemetry(ILogger logger, string deviceId)
        {
            _logger = logger;
            _deviceId = deviceId;

            ActivatePresence = false;
            TelemetryActive = true;

            int peakFrequencyInTicks = Convert.ToInt32(Math.Ceiling((double)PEAK_FREQUENCY_IN_SECONDS / REPORT_FREQUENCY_IN_SECONDS));

            ZWaveDeviceManager.ZwaveAdapter.ZWaveNotification += TelemetryNotification;
        }


        private void TelemetryNotification(ZWNotification notification)
        {
            byte nodeId = notification.GetNodeId();
            uint installationId = notification.GetHomeId();
            AdapterLib.NotificationType notificationType = notification.GetType();

            // Catch only Value changed event 
            if (notificationType == AdapterLib.NotificationType.ValueChanged || notificationType != AdapterLib.NotificationType.Notification)
            {


                ZWValueID valueID = notification.GetValueID();
                string serializedValue = JsonConvert.SerializeObject(notification, new ZWaveNotificationJsonConverter());
                this._logger.LogInfo(serializedValue);


                ZWaveSensorTelemetryData telemetry = new ZWaveSensorTelemetryData();
                telemetry.Time = DateTime.Now;
                telemetry.DeviceId = this._deviceId;
                telemetry.ValueLabel = valueID.ValueLabel;
                telemetry.ValueUnits = valueID.ValueUnits;

                telemetry.Type = valueID.Type.ToString();
                if (valueID != null)
                {
                    try
                    {
                        telemetry.Value = Convert.ToDouble(valueID.Value);

                    }
                    catch (Exception ex)
                    {
                        this._logger.LogError("Error converting value {0} to double: {1}", new object[] { valueID.Value, ex.Message });
                        return;
                    }
                }

                if (TelemetryActive)
                    TelemetryQueue.Enqueue(telemetry);
            }

        }

        public async Task SendEventsAsync(CancellationToken token, Func<object, Task> sendMessageAsync)
        {
           
            
            while (!token.IsCancellationRequested)
            {
                while (TelemetryQueue.Count > 0)
                {
                    var monitorData = TelemetryQueue.Dequeue();
                    if (TelemetryActive && monitorData != null)
                    {
                        //_logger.LogInfo("Sending " + messageBody + " for Device: " + _deviceId);

                        await sendMessageAsync(monitorData);
                    }
                }
                await Task.Delay(TimeSpan.FromSeconds(REPORT_FREQUENCY_IN_SECONDS), token);
            }
        }
    }
}
