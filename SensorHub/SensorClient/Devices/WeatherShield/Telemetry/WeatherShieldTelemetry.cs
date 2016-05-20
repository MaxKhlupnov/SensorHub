using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RemoteMonitoring.Telemetry;
using RemoteMonitoring.Logging;
using RemoteMonitoring.Devices;
using System.Threading;

using SensorClient.DataModel;

namespace SensorClient.Devices.WeatherShield.Telemetry
{
    public class WeatherShieldTelemetry : ITelemetry
    {
        private readonly ILogger _logger;
        private readonly WeatherShieldDevice _device;

        private const int REPORT_FREQUENCY_IN_SECONDS = 60;
        private const int PEAK_FREQUENCY_IN_SECONDS = 90;

        public bool TelemetryActive { get; set; }


        public WeatherShieldTelemetry(ILogger logger, WeatherShieldDevice device)
        {
            this._logger = logger;
            this._device = device;          
            this.TelemetryActive = !string.IsNullOrWhiteSpace(device.HostName) && !string.IsNullOrWhiteSpace(device.PrimaryAuthKey);          
        }

        public async Task SendEventsAsync(CancellationToken token, Func<object, Task> sendMessageAsync)
        {
          
            
            while (!token.IsCancellationRequested)
            {
                if (TelemetryActive)
                {
                    WeatherShieldTelemetryData sensorsTelemetryData = await ReadWeatherShieldSensors();
                    if (sensorsTelemetryData == null)
                    {
                        this._logger.LogWarning("No telemetry data for device {0};", new object[] { _device.DeviceID });
                        return;
                    }
                    try
                    {
                        this._logger.LogInfo("Sending telemetry for device {0};", new object[] { _device.DeviceID });
                        await sendMessageAsync(sensorsTelemetryData);
                    }
                    catch (Exception ex)
                    {
                        this._logger.LogError("Error {0} sending telemetry for device {1}; ",
                            new object[] { ex.Message, _device.DeviceID });
                    }
                }

            }
        }

        private async Task<WeatherShieldTelemetryData> ReadWeatherShieldSensors()
        {
 
            WeatherShieldTelemetryData sensorsTelemetryData = new WeatherShieldTelemetryData();
            sensorsTelemetryData.DeviceId = this._device.DeviceID;

            bool hasData = false;
            SensorTelemetryData measurement = null;
            foreach (AbstractSensor sensor in this._device.DeviceSensors)
            {
                measurement = await sensorsTelemetryData.ReadSensorTelemetry(sensor);
                if (measurement != null)
                {
                    hasData = true;                   
                }
            }


            if (hasData)
                return sensorsTelemetryData;
            else
                return null;
        }

    }

   
}
