using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RemoteMonitoring.Devices;
using RemoteMonitoring.Logging;

namespace Platfrom
{
    public interface IScoutConfig
    {
        void ProcessNewDiscoveryResults(List<IDevice> deviceList);
        void SetDeviceDriverParams(IDevice device, List<string> paramList);
        List<string> GetDeviceDriverParams(IDevice device);
        string GetConfSetting(string paramName);
        string GetPrivateConfSetting(string paramName);
    }
}
