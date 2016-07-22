using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Newtonsoft.Json;
using AdapterLib;

namespace SensorClient.Devices.ZWaveMultisensor.Telemetry
{

  

    public class ZWaveNotificationJsonConverter : JsonConverter
    {
        public override bool CanConvert(Type objectType)
        {
            return typeof(ZWNotification) == objectType;
                
        }

        public override object ReadJson(JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
        {
            throw new NotImplementedException();
        }

        public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
        {
            var notification = value as ZWNotification;
            writer.WriteStartObject();
                writer.WritePropertyName("HomeId");
                writer.WriteValue(notification.GetHomeId());
                
                writer.WritePropertyName("NodeId");
                writer.WriteValue(notification.GetNodeId());

                writer.WritePropertyName("Type");
                writer.WriteValue(notification.GetType().ToString());

                writer.WritePropertyName("Code");
                writer.WriteValue(notification.GetCode().ToString());

           ZWValueID valueID = notification.GetValueID();
            if (valueID != null)
            {                               
                writer.WritePropertyName("ValueID");
                 string serializedValue = JsonConvert.SerializeObject(valueID, new ZWaveValueIDJsonConverter());
              //  string serializedValue = "{\"ValueGenre\": \"" + valueID.Genre + "  \"}";
                writer.WriteRawValue(serializedValue);               
                 
            }


            writer.WriteEndObject();


        }

    }

    public class ZWaveValueIDJsonConverter : JsonConverter
    {
        public override bool CanConvert(Type objectType)
        {
            return typeof(ZWValueID) == objectType;
        }

        public override object ReadJson(JsonReader reader, Type objectType, object existingValue, JsonSerializer serializer)
        {
            throw new NotImplementedException();
        }

        public override void WriteJson(JsonWriter writer, object value, JsonSerializer serializer)
        {
            var ValueID = value as ZWValueID;
            writer.WriteStartObject();

            writer.WritePropertyName("Genre");
            writer.WriteValue(ValueID.Genre.ToString());

            writer.WritePropertyName("Type");
            writer.WriteValue(ValueID.Type.ToString());

            writer.WritePropertyName("ValueLabel");
            writer.WriteValue(ValueID.ValueLabel);

            writer.WritePropertyName("ValueUnits");
            writer.WriteValue(ValueID.ValueUnits);

            writer.WritePropertyName("Value");
            writer.WriteValue(ValueID.Value);

            writer.WriteEndObject();
        }
    }

}
