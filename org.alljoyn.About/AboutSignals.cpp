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
#include "pch.h"

using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Devices::AllJoyn;
using namespace Windows::Foundation;
using namespace org::alljoyn::About;

void AboutSignals::Initialize(_In_ alljoyn_busobject busObject, _In_ alljoyn_sessionid sessionId)
{
    m_busObject = busObject;
    m_sessionId = sessionId;

    auto interfaceDefinition = alljoyn_busattachment_getinterface(alljoyn_busobject_getbusattachment(busObject), "org.alljoyn.About");
    alljoyn_interfacedescription_getmember(interfaceDefinition, "Announce", &m_memberAnnounce);
}

void AboutSignals::Announce(_In_ uint16 interfaceMemberVersion, _In_ uint16 interfaceMemberPort, _In_ Windows::Foundation::Collections::IVectorView<AboutObjectDescriptionItem^>^ interfaceMemberObjectDescription, _In_ Windows::Foundation::Collections::IMapView<Platform::String^,Platform::Object^>^ interfaceMemberMetaData)
{
    if (nullptr == m_busObject)
    {
        return;
    }

    size_t argCount = 4;
    alljoyn_msgarg arguments = alljoyn_msgarg_array_create(argCount);
    (void)TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(arguments, 0), "q", interfaceMemberVersion);
    (void)TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(arguments, 1), "q", interfaceMemberPort);
    (void)TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(arguments, 2), "a(sas)", interfaceMemberObjectDescription);
    (void)TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(arguments, 3), "a{sv}", interfaceMemberMetaData);
    
    alljoyn_busobject_signal(
        m_busObject, 
        NULL,  // Generated code only supports broadcast signals.
        m_sessionId,
        m_memberAnnounce,
        arguments,
        argCount, 
        0, // A signal with a TTL of 0 will be sent to every member of the session, regardless of how long it takes to deliver the message
        ALLJOYN_MESSAGE_FLAG_GLOBAL_BROADCAST, // Broadcast to everyone in the session.
        NULL); // The generated code does not need the generated signal message

    alljoyn_msgarg_destroy(arguments);
}

void AboutSignals::CallAnnounceReceived(_In_ AboutSignals^ sender, _In_ AboutAnnounceReceivedEventArgs^ args)
{
    AnnounceReceived(sender, args);
}

