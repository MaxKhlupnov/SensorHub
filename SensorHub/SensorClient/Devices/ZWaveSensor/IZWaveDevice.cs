using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RemoteMonitoring.Devices;

namespace SensorClient.Devices.ZWaveSensor
{
    public interface IZWaveDevice : IDevice
    {
        byte nodeID { get; set; }
        uint homeID { get; set; }
    }
}
