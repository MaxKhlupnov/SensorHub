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
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Devices::AllJoyn;
using namespace org::alljoyn::Config;

// Note: Unlike an Interface implementation, which provides a single handler for each member, the event
// model allows for 0 or more listeners to be registered. The EventAdapter implementation deals with this
// difference by implementing a last-writer-wins policy. The lack of any return value (i.e., 0 listeners)
// is handled by returning a null result.

// Methods
IAsyncOperation<ConfigFactoryResetResult^>^ ConfigServiceEventAdapter::FactoryResetAsync(_In_ AllJoynMessageInfo^ info)
{
    auto args = ref new ConfigFactoryResetCalledEventArgs(info);
    FactoryResetCalled(this, args);
    return ConfigFactoryResetCalledEventArgs::GetResultAsync(args);
}

IAsyncOperation<ConfigGetConfigurationsResult^>^ ConfigServiceEventAdapter::GetConfigurationsAsync(_In_ AllJoynMessageInfo^ info, _In_ Platform::String^ interfaceMemberLanguageTag)
{
    auto args = ref new ConfigGetConfigurationsCalledEventArgs(info, interfaceMemberLanguageTag);
    GetConfigurationsCalled(this, args);
    return ConfigGetConfigurationsCalledEventArgs::GetResultAsync(args);
}

IAsyncOperation<ConfigResetConfigurationsResult^>^ ConfigServiceEventAdapter::ResetConfigurationsAsync(_In_ AllJoynMessageInfo^ info, _In_ Platform::String^ interfaceMemberLanguageTag, _In_ Windows::Foundation::Collections::IVectorView<Platform::String^>^ interfaceMemberFieldList)
{
    auto args = ref new ConfigResetConfigurationsCalledEventArgs(info, interfaceMemberLanguageTag, interfaceMemberFieldList);
    ResetConfigurationsCalled(this, args);
    return ConfigResetConfigurationsCalledEventArgs::GetResultAsync(args);
}

IAsyncOperation<ConfigRestartResult^>^ ConfigServiceEventAdapter::RestartAsync(_In_ AllJoynMessageInfo^ info)
{
    auto args = ref new ConfigRestartCalledEventArgs(info);
    RestartCalled(this, args);
    return ConfigRestartCalledEventArgs::GetResultAsync(args);
}

IAsyncOperation<ConfigSetPasscodeResult^>^ ConfigServiceEventAdapter::SetPasscodeAsync(_In_ AllJoynMessageInfo^ info, _In_ Platform::String^ interfaceMemberDaemonRealm, _In_ Windows::Foundation::Collections::IVectorView<byte>^ interfaceMemberNewPasscode)
{
    auto args = ref new ConfigSetPasscodeCalledEventArgs(info, interfaceMemberDaemonRealm, interfaceMemberNewPasscode);
    SetPasscodeCalled(this, args);
    return ConfigSetPasscodeCalledEventArgs::GetResultAsync(args);
}

IAsyncOperation<ConfigUpdateConfigurationsResult^>^ ConfigServiceEventAdapter::UpdateConfigurationsAsync(_In_ AllJoynMessageInfo^ info, _In_ Platform::String^ interfaceMemberLanguageTag, _In_ Windows::Foundation::Collections::IMapView<Platform::String^,Platform::Object^>^ interfaceMemberConfigMap)
{
    auto args = ref new ConfigUpdateConfigurationsCalledEventArgs(info, interfaceMemberLanguageTag, interfaceMemberConfigMap);
    UpdateConfigurationsCalled(this, args);
    return ConfigUpdateConfigurationsCalledEventArgs::GetResultAsync(args);
}

// Property Reads
IAsyncOperation<ConfigGetVersionResult^>^ ConfigServiceEventAdapter::GetVersionAsync(_In_ AllJoynMessageInfo^ info)
{
    auto args = ref new ConfigGetVersionRequestedEventArgs(info);
    GetVersionRequested(this, args);
    return ConfigGetVersionRequestedEventArgs::GetResultAsync(args);
}

// Property Writes
