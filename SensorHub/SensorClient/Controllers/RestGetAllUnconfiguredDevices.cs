using Devkoes.Restup.WebServer.Attributes;
using Devkoes.Restup.WebServer.Models.Schemas;
using System;
using System.Threading.Tasks;
using Windows.Foundation;

using Devkoes.Restup.WebServer.Rest.Models.Contracts;
using SensorClient.DataModel;

namespace SensorClient.Controllers
{
    [RestController(InstanceCreationType.PerCall)]
    public sealed class RestGetAllUnconfiguredDevices
    {

        /// You can use the normal async/await syntax. Note that the type parameter of the returning Task'T should be
        /// one of the supported responses.
        /// </summary>
        /// <returns></returns>
        [UriFormat("/GetAllUnconfiguredDevices")]
        public IAsyncOperation<IGetResponse> GetConfiguredDevices()
        {
            RestDeviceDataModel[] retValue = new RestDeviceDataModel[] {
            new RestDeviceDataModel()
            {
                DeviceId = "2_123456",
                DeviceModuleName = "ZWaveMultisensor",
                DeviceFriendlyName = "Multisensor",
                DeviceLocation = "Нескучный сад"
            }
            };

            return Task.FromResult<IGetResponse>(new GetResponse(GetResponse.ResponseStatus.OK,
                retValue
                )).AsAsyncOperation();
        }
    }
}
