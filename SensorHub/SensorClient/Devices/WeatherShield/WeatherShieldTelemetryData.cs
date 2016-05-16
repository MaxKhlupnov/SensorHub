using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SensorClient.DataModel;

namespace SensorClient.Devices.WeatherShield.Telemetry
{
   public class WeatherShieldTelemetryData {    
        public string DeviceId { get; set; }
        public double Temperature { get; set; }
        public double Humidity { get; set; }
        public double? Pressure { get; set; }

        internal async Task<SensorTelemetryData> ReadSensorTelemetry(AbstractSensor sensor)
        {
            if (sensor != null)
            {
                SensorTelemetryData data =  await sensor.DoMeasure();
                if (typeof(HumiditySensor).Equals(sensor.GetType()))
                    this.Humidity = data.Value;
                else if (typeof(TemperatureSensor).Equals(sensor.GetType()))
                    this.Temperature = data.Value;
                else if (typeof(PressureSensor).Equals(sensor.GetType()))
                    this.Pressure = data.Value;

                return data;
            }
                return null;
        }
        
    }
}
