using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.DeviceSchema;
using Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Models;

namespace Microsoft.Azure.Devices.Applications.RemoteMonitoring.Common.Factory
{
    public abstract class AbstractDeviceFactory
    {

        protected class Location
        {
            public string Title { get; set; }
            public double Latitude { get; set; }
            public double Longitude { get; set; }

            public Location(string title, double latitude, double longitude)
            {
                Latitude = latitude;
                Longitude = longitude;
                Title = title;
            }


        }

        private static int GetIntBasedOnString(string input, int maxValueExclusive)
        {
            int hash = input.GetHashCode();

            //Keep the result positive
            if (hash < 0)
            {
                hash = -hash;
            }

            return hash % maxValueExclusive;
        }

       // public abstract void AssignDeviceProperties();

        protected void AssignTelemetry(string name, string displayName, string type)
        {
            dynamic telemetry = CommandSchemaHelper.CreateNewTelemetry(name, displayName, type);
            CommandSchemaHelper.AddTelemetryToDevice(this, telemetry);
        }

        protected void AssignCommand(string commandName)
        {
            dynamic command = CommandSchemaHelper.CreateNewCommand(commandName);
            CommandSchemaHelper.AddCommandToDevice(this, command);
        }
  
    }
}
