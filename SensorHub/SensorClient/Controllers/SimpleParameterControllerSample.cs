﻿
using Devkoes.Restup.WebServer.Attributes;
using Devkoes.Restup.WebServer.Models.Schemas;
using Devkoes.Restup.WebServer.Rest.Models.Contracts;
using SensorClient.DataModel;

namespace SensorClient.Controllers
{
    [RestController(InstanceCreationType.Singleton)]
    public sealed class SimpleParameterControllerSample
    {       
        /// <summary>
        /// Make sure the number of parameters in your UriFormat match the parameters in your method and
        /// the names (case sensitive) and order are respected.
        /// </summary>
        [UriFormat("/simpleparameter/{id}/property/{propName}")]
        public IGetResponse GetWithSimpleParameters(int id, string propName)
        {
            return new GetResponse(
                GetResponse.ResponseStatus.OK,
                new RestDeviceDataModel()
                {
                    DeviceId = id.ToString(),
                    DeviceFriendlyName = propName
                });
        }
    }
}
