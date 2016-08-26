using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using Devkoes.Restup.WebServer.File;
using Devkoes.Restup.WebServer.Http;
using Devkoes.Restup.WebServer.Rest;

using SensorClient.Devices;
using SensorClient.Controllers;
using SensorClient.Common;

using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Configurations;

namespace SensorClient
{
    public static class AppController
    {

        private static CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

        private static readonly IConfigurationProvider _configProvider = new ConfigurationProvider();
        public static TraceLogger Logger { get; private set; }
        public static ZWaveDeviceManager DeviceManager { get; private set; }

        public static HttpServer httpServer { get; private set; }


        internal static async Task Initialize()
        {
            if (Logger == null)
            {
                Logger = new TraceLogger();
            }

            if (DeviceManager == null)
            {
                DeviceManager = new ZWaveDeviceManager(_configProvider, Logger, cancellationTokenSource.Token);
            }

            await InitializeWebServer();
        }

        private static async Task InitializeWebServer()
        {
            
            httpServer = new HttpServer(8800);

            var restRouteHandler = new RestRouteHandler();

            restRouteHandler.RegisterController<RestGetConfiguredDevices>();
            restRouteHandler.RegisterController<SimpleParameterControllerSample>();
            /* 
             restRouteHandler.RegisterController<FromContentControllerSample>();
             restRouteHandler.RegisterController<PerCallControllerSample>();
             restRouteHandler.RegisterController<SimpleParameterControllerSample>();
             restRouteHandler.RegisterController<SingletonControllerSample>();
             restRouteHandler.RegisterController<ThrowExceptionControllerSample>();
             restRouteHandler.RegisterController<WithResponseContentControllerSample>();*/

            httpServer.RegisterRoute("webapp", restRouteHandler);

            httpServer.RegisterRoute(new StaticFileRouteHandler(@"DashboardWeb\wwwroot"));
            await httpServer.StartServerAsync();

            // Dont release deferral, otherwise app will stop
        }
    }
}
