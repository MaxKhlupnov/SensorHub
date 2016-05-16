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

namespace org { namespace alljoyn { namespace Config {

public interface class IConfigService
{
public:
    // Implement this function to handle calls to the FactoryReset method.
    Windows::Foundation::IAsyncOperation<ConfigFactoryResetResult^>^ FactoryResetAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info );

    // Implement this function to handle calls to the GetConfigurations method.
    Windows::Foundation::IAsyncOperation<ConfigGetConfigurationsResult^>^ GetConfigurationsAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info , _In_ Platform::String^ interfaceMemberLanguageTag);

    // Implement this function to handle calls to the ResetConfigurations method.
    Windows::Foundation::IAsyncOperation<ConfigResetConfigurationsResult^>^ ResetConfigurationsAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info , _In_ Platform::String^ interfaceMemberLanguageTag, _In_ Windows::Foundation::Collections::IVectorView<Platform::String^>^ interfaceMemberFieldList);

    // Implement this function to handle calls to the Restart method.
    Windows::Foundation::IAsyncOperation<ConfigRestartResult^>^ RestartAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info );

    // Implement this function to handle calls to the SetPasscode method.
    Windows::Foundation::IAsyncOperation<ConfigSetPasscodeResult^>^ SetPasscodeAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info , _In_ Platform::String^ interfaceMemberDaemonRealm, _In_ Windows::Foundation::Collections::IVectorView<byte>^ interfaceMemberNewPasscode);

    // Implement this function to handle calls to the UpdateConfigurations method.
    Windows::Foundation::IAsyncOperation<ConfigUpdateConfigurationsResult^>^ UpdateConfigurationsAsync(_In_ Windows::Devices::AllJoyn::AllJoynMessageInfo^ info , _In_ Platform::String^ interfaceMemberLanguageTag, _In_ Windows::Foundation::Collections::IMapView<Platform::String^,Platform::Object^>^ interfaceMemberConfigMap);

    // Implement this function to handle requests for the value of the Version property.
    //
    // Currently, info will always be null, because no information is available about the requestor.
    Windows::Foundation::IAsyncOperation<ConfigGetVersionResult^>^ GetVersionAsync(Windows::Devices::AllJoyn::AllJoynMessageInfo^ info);

};

} } } 
