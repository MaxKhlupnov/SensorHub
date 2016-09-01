using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using RemoteMonitoring.Devices;
using RemoteMonitoring.Logging;

namespace Platfrom
{
    public interface IScout
    {
        void Init(string baseUrl, string baseDir, IScoutConfig platform, ILogger logger);
        List<IDevice> GetDevices();

        void Dispose();
    }
}
