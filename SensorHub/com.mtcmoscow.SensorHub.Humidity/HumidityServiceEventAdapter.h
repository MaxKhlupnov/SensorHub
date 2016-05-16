//-----------------------------------------------------------------------------
// <auto-generated> 
//   This code was generated by a tool. 
// 
//   Changes to this file may cause incorrect behavior and will be lost if  
//   the code is regenerated.
//
//   Tool: AllJoynCodeGenerator.exe
//
//   This tool is located in the Windows 10 SDK and the Windows 10 AllJoyn 
//   Visual Studio Extension in the Visual Studio Gallery.  
//
//   The generated code should be packaged in a Windows 10 C++/CX Runtime  
//   Component which can be consumed in any UWP-supported language using 
//   APIs that are available in Windows.Devices.AllJoyn.
//
//   Using AllJoynCodeGenerator - Invoke the following command with a valid 
//   Introspection XML file and a writable output directory:
//     AllJoynCodeGenerator -i <INPUT XML FILE> -o <OUTPUT DIRECTORY>
// </auto-generated>
//-----------------------------------------------------------------------------
#pragma once

namespace com { namespace mtcmoscow { namespace SensorHub { namespace Humidity {

// This class, and the associated EventArgs classes, exist for the benefit of JavaScript developers who
// do not have the ability to implement IHumidityService. Instead, HumidityServiceEventAdapter
// provides the Interface implementation and exposes a set of compatible events to the developer.
public ref class HumidityServiceEventAdapter sealed : [Windows::Foundation::Metadata::Default] IHumidityService
{
public:
    // Method Invocation Events

    // Property Read Events
    event Windows::Foundation::TypedEventHandler<HumidityServiceEventAdapter^, HumidityGetRHRequestedEventArgs^>^ GetRHRequested;
    
    // Property Write Events
    event Windows::Foundation::TypedEventHandler<HumidityServiceEventAdapter^, HumiditySetRHRequestedEventArgs^>^ SetRHRequested;

    // IHumidityService Implementation

    virtual Windows::Foundation::IAsyncOperation<HumidityGetRHResult^>^ GetRHAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info);

    virtual Windows::Foundation::IAsyncOperation<HumiditySetRHResult^>^ SetRHAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info, _In_ double value);
};

} } } } 
