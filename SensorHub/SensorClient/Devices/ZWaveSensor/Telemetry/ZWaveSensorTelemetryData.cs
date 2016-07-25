
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using AdapterLib;

namespace SensorClient.Devices.ZWaveSensor.Telemetry
{
    public class ZWaveSensorTelemetryData
    {

        public string DeviceId { get; set; }
        public DateTime Time { get; set; }
        public string ValueLabel { get; set; }
        public string Type { get; set; }
        public string ValueUnits { get; set; }
        public double? Value { get; set; }
    }
}
