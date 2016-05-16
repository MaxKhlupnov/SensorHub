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

using namespace concurrency;
using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Devices::AllJoyn;
using namespace org::alljoyn::Config;

std::map<alljoyn_interfacedescription, WeakReference*> ConfigConsumer::SourceInterfaces;

ConfigConsumer::ConfigConsumer(AllJoynBusAttachment^ busAttachment)
    : m_busAttachment(busAttachment),
    m_proxyBusObject(nullptr),
    m_busObject(nullptr),
    m_sessionListener(nullptr),
    m_sessionId(0)
{
    m_weak = new WeakReference(this);
    m_signals = ref new ConfigSignals();
    m_nativeBusAttachment = AllJoynHelpers::GetInternalBusAttachment(m_busAttachment);
}

ConfigConsumer::~ConfigConsumer()
{
    AllJoynBusObjectManager::ReleaseBusObject(m_nativeBusAttachment, AllJoynHelpers::PlatformToMultibyteString(ServiceObjectPath).data());
    if (SessionListener != nullptr)
    {
        alljoyn_busattachment_setsessionlistener(m_nativeBusAttachment, m_sessionId, nullptr);
        alljoyn_sessionlistener_destroy(SessionListener);
    }
    if (nullptr != ProxyBusObject)
    {
        alljoyn_proxybusobject_destroy(ProxyBusObject);
    }
    delete m_weak;
}

void ConfigConsumer::OnSessionLost(_In_ alljoyn_sessionid sessionId, _In_ alljoyn_sessionlostreason reason)
{
    if (sessionId == m_sessionId)
    {
        AllJoynSessionLostEventArgs^ args = ref new AllJoynSessionLostEventArgs(static_cast<AllJoynSessionLostReason>(reason));
        SessionLost(this, args);
    }
}

void ConfigConsumer::OnSessionMemberAdded(_In_ alljoyn_sessionid sessionId, _In_ PCSTR uniqueName)
{
    if (sessionId == m_sessionId)
    {
        auto args = ref new AllJoynSessionMemberAddedEventArgs(AllJoynHelpers::MultibyteToPlatformString(uniqueName));
        SessionMemberAdded(this, args);
    }
}

void ConfigConsumer::OnSessionMemberRemoved(_In_ alljoyn_sessionid sessionId, _In_ PCSTR uniqueName)
{
    if (sessionId == m_sessionId)
    {
        auto args = ref new AllJoynSessionMemberRemovedEventArgs(AllJoynHelpers::MultibyteToPlatformString(uniqueName));
        SessionMemberRemoved(this, args);
    }
}

QStatus ConfigConsumer::AddSignalHandler(_In_ alljoyn_busattachment busAttachment, _In_ alljoyn_interfacedescription interfaceDescription, _In_ PCSTR methodName, _In_ alljoyn_messagereceiver_signalhandler_ptr handler)
{
    alljoyn_interfacedescription_member member;
    if (!alljoyn_interfacedescription_getmember(interfaceDescription, methodName, &member))
    {
        return ER_BUS_INTERFACE_NO_SUCH_MEMBER;
    }

    return alljoyn_busattachment_registersignalhandler(busAttachment, handler, member, NULL);
}

IAsyncOperation<ConfigJoinSessionResult^>^ ConfigConsumer::JoinSessionAsync(
    _In_ AllJoynServiceInfo^ serviceInfo, _Inout_ ConfigWatcher^ watcher)
{
    return create_async([serviceInfo, watcher]() -> ConfigJoinSessionResult^
    {
        auto result = ref new ConfigJoinSessionResult();
        result->Consumer = ref new ConfigConsumer(watcher->BusAttachment);
        result->Status = result->Consumer->JoinSession(serviceInfo);
        return result;
    });
}

IAsyncOperation<ConfigFactoryResetResult^>^ ConfigConsumer::FactoryResetAsync()
{
    return create_async([this]() -> ConfigFactoryResetResult^
    {
        auto result = ref new ConfigFactoryResetResult();
        
        alljoyn_message message = alljoyn_message_create(m_nativeBusAttachment);
        size_t argCount = 0;
        alljoyn_msgarg inputs = alljoyn_msgarg_array_create(argCount);

        QStatus status = ER_OK;
        if (ER_OK == status)
        {
            status = alljoyn_proxybusobject_methodcall(
                ProxyBusObject,
                "org.alljoyn.Config",
                "FactoryReset",
                inputs,
                argCount,
                message,
                c_MessageTimeoutInMilliseconds,
                0);
        }
        result->Status = static_cast<int>(status);
        
        alljoyn_message_destroy(message);
        alljoyn_msgarg_destroy(inputs);

        return result;
    });
}
IAsyncOperation<ConfigGetConfigurationsResult^>^ ConfigConsumer::GetConfigurationsAsync(_In_ Platform::String^ interfaceMemberLanguageTag)
{
    return create_async([this, interfaceMemberLanguageTag]() -> ConfigGetConfigurationsResult^
    {
        auto result = ref new ConfigGetConfigurationsResult();
        
        alljoyn_message message = alljoyn_message_create(m_nativeBusAttachment);
        size_t argCount = 1;
        alljoyn_msgarg inputs = alljoyn_msgarg_array_create(argCount);

        QStatus status = ER_OK;
        status = static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(inputs, 0), "s", interfaceMemberLanguageTag));
	
        if (ER_OK == status)
        {
            status = alljoyn_proxybusobject_methodcall(
                ProxyBusObject,
                "org.alljoyn.Config",
                "GetConfigurations",
                inputs,
                argCount,
                message,
                c_MessageTimeoutInMilliseconds,
                0);
        }
        result->Status = static_cast<int>(status);
        if (ER_OK == status) 
        {
            result->Status = AllJoynStatus::Ok;
            Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ argument0;
            status = static_cast<QStatus>(TypeConversionHelpers::GetAllJoynMessageArg(alljoyn_message_getarg(message, 0), "a{sv}", &argument0));
            result->Languages = argument0;

            if (ER_OK != status)
            {
                result->Status = static_cast<int>(status);
            }
        }
        else if (ER_BUS_REPLY_IS_ERROR_MESSAGE == status)
        {
            alljoyn_msgarg errorArg = alljoyn_message_getarg(message, 1);
            if (nullptr != errorArg)
            {
                uint16 errorStatus;
                status = alljoyn_msgarg_get_uint16(errorArg, &errorStatus);
                if (ER_OK == status)
                {
                    status = static_cast<QStatus>(errorStatus);
                }
            }
            result->Status = static_cast<int>(status);
        }
        
        alljoyn_message_destroy(message);
        alljoyn_msgarg_destroy(inputs);

        return result;
    });
}
IAsyncOperation<ConfigResetConfigurationsResult^>^ ConfigConsumer::ResetConfigurationsAsync(_In_ Platform::String^ interfaceMemberLanguageTag, _In_ Windows::Foundation::Collections::IVectorView<Platform::String^>^ interfaceMemberFieldList)
{
    return create_async([this, interfaceMemberLanguageTag, interfaceMemberFieldList]() -> ConfigResetConfigurationsResult^
    {
        auto result = ref new ConfigResetConfigurationsResult();
        
        alljoyn_message message = alljoyn_message_create(m_nativeBusAttachment);
        size_t argCount = 2;
        alljoyn_msgarg inputs = alljoyn_msgarg_array_create(argCount);

        QStatus status = ER_OK;
        status = static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(inputs, 0), "s", interfaceMemberLanguageTag));
	
        if (ER_OK == status)
        {
            status = static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(inputs, 1), "as", interfaceMemberFieldList));
        }
	
        if (ER_OK == status)
        {
            status = alljoyn_proxybusobject_methodcall(
                ProxyBusObject,
                "org.alljoyn.Config",
                "ResetConfigurations",
                inputs,
                argCount,
                message,
                c_MessageTimeoutInMilliseconds,
                0);
        }
        result->Status = static_cast<int>(status);
        
        alljoyn_message_destroy(message);
        alljoyn_msgarg_destroy(inputs);

        return result;
    });
}
IAsyncOperation<ConfigRestartResult^>^ ConfigConsumer::RestartAsync()
{
    return create_async([this]() -> ConfigRestartResult^
    {
        auto result = ref new ConfigRestartResult();
        
        alljoyn_message message = alljoyn_message_create(m_nativeBusAttachment);
        size_t argCount = 0;
        alljoyn_msgarg inputs = alljoyn_msgarg_array_create(argCount);

        QStatus status = ER_OK;
        if (ER_OK == status)
        {
            status = alljoyn_proxybusobject_methodcall(
                ProxyBusObject,
                "org.alljoyn.Config",
                "Restart",
                inputs,
                argCount,
                message,
                c_MessageTimeoutInMilliseconds,
                0);
        }
        result->Status = static_cast<int>(status);
        
        alljoyn_message_destroy(message);
        alljoyn_msgarg_destroy(inputs);

        return result;
    });
}
IAsyncOperation<ConfigSetPasscodeResult^>^ ConfigConsumer::SetPasscodeAsync(_In_ Platform::String^ interfaceMemberDaemonRealm, _In_ Windows::Foundation::Collections::IVectorView<byte>^ interfaceMemberNewPasscode)
{
    return create_async([this, interfaceMemberDaemonRealm, interfaceMemberNewPasscode]() -> ConfigSetPasscodeResult^
    {
        auto result = ref new ConfigSetPasscodeResult();
        
        alljoyn_message message = alljoyn_message_create(m_nativeBusAttachment);
        size_t argCount = 2;
        alljoyn_msgarg inputs = alljoyn_msgarg_array_create(argCount);

        QStatus status = ER_OK;
        status = static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(inputs, 0), "s", interfaceMemberDaemonRealm));
	
        if (ER_OK == status)
        {
            status = static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(inputs, 1), "ay", interfaceMemberNewPasscode));
        }
	
        if (ER_OK == status)
        {
            status = alljoyn_proxybusobject_methodcall(
                ProxyBusObject,
                "org.alljoyn.Config",
                "SetPasscode",
                inputs,
                argCount,
                message,
                c_MessageTimeoutInMilliseconds,
                0);
        }
        result->Status = static_cast<int>(status);
        
        alljoyn_message_destroy(message);
        alljoyn_msgarg_destroy(inputs);

        return result;
    });
}
IAsyncOperation<ConfigUpdateConfigurationsResult^>^ ConfigConsumer::UpdateConfigurationsAsync(_In_ Platform::String^ interfaceMemberLanguageTag, _In_ Windows::Foundation::Collections::IMapView<Platform::String^,Platform::Object^>^ interfaceMemberConfigMap)
{
    return create_async([this, interfaceMemberLanguageTag, interfaceMemberConfigMap]() -> ConfigUpdateConfigurationsResult^
    {
        auto result = ref new ConfigUpdateConfigurationsResult();
        
        alljoyn_message message = alljoyn_message_create(m_nativeBusAttachment);
        size_t argCount = 2;
        alljoyn_msgarg inputs = alljoyn_msgarg_array_create(argCount);

        QStatus status = ER_OK;
        status = static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(inputs, 0), "s", interfaceMemberLanguageTag));
	
        if (ER_OK == status)
        {
            status = static_cast<QStatus>(TypeConversionHelpers::SetAllJoynMessageArg(alljoyn_msgarg_array_element(inputs, 1), "a{sv}", interfaceMemberConfigMap));
        }
	
        if (ER_OK == status)
        {
            status = alljoyn_proxybusobject_methodcall(
                ProxyBusObject,
                "org.alljoyn.Config",
                "UpdateConfigurations",
                inputs,
                argCount,
                message,
                c_MessageTimeoutInMilliseconds,
                0);
        }
        result->Status = static_cast<int>(status);
        
        alljoyn_message_destroy(message);
        alljoyn_msgarg_destroy(inputs);

        return result;
    });
}

IAsyncOperation<ConfigGetVersionResult^>^ ConfigConsumer::GetVersionAsync()
{
    return create_async([this]() -> ConfigGetVersionResult^
    {
        PropertyGetContext<uint16> getContext;
        
        alljoyn_proxybusobject_getpropertyasync(
            ProxyBusObject,
            "org.alljoyn.Config",
            "Version",
            [](QStatus status, alljoyn_proxybusobject obj, const alljoyn_msgarg value, void* context)
            {
                UNREFERENCED_PARAMETER(obj);
                auto propertyContext = static_cast<PropertyGetContext<uint16>*>(context);

                if (ER_OK == status)
                {
                    uint16 argument;
                    status = static_cast<QStatus>(TypeConversionHelpers::GetAllJoynMessageArg(value, "q", &argument));

                    propertyContext->SetValue(argument);
                }
                propertyContext->SetStatus(status);
                propertyContext->SetEvent();
            },
            c_MessageTimeoutInMilliseconds,
            &getContext);

        getContext.Wait();

        auto result = ref new ConfigGetVersionResult();
        result->Status = getContext.GetStatus();
        result->Version = getContext.GetValue();
        return result;
    });
}

void ConfigConsumer::OnPropertyChanged(_In_ alljoyn_proxybusobject obj, _In_ PCSTR interfaceName, _In_ const alljoyn_msgarg changed, _In_ const alljoyn_msgarg invalidated)
{
    UNREFERENCED_PARAMETER(obj);
    UNREFERENCED_PARAMETER(interfaceName);
    UNREFERENCED_PARAMETER(changed);
    UNREFERENCED_PARAMETER(invalidated);
}

int32 ConfigConsumer::JoinSession(_In_ AllJoynServiceInfo^ serviceInfo)
{
    alljoyn_sessionlistener_callbacks callbacks =
    {
        AllJoynHelpers::SessionLostHandler<ConfigConsumer>,
        AllJoynHelpers::SessionMemberAddedHandler<ConfigConsumer>,
        AllJoynHelpers::SessionMemberRemovedHandler<ConfigConsumer>
    };

    alljoyn_busattachment_enableconcurrentcallbacks(AllJoynHelpers::GetInternalBusAttachment(m_busAttachment));

    SessionListener = alljoyn_sessionlistener_create(&callbacks, m_weak);
    alljoyn_sessionopts sessionOpts = alljoyn_sessionopts_create(ALLJOYN_TRAFFIC_TYPE_MESSAGES, true, ALLJOYN_PROXIMITY_ANY, ALLJOYN_TRANSPORT_ANY);

    std::vector<char> sessionNameUtf8 = AllJoynHelpers::PlatformToMultibyteString(serviceInfo->UniqueName);
    RETURN_IF_QSTATUS_ERROR(alljoyn_busattachment_joinsession(
        m_nativeBusAttachment,
        &sessionNameUtf8[0],
        serviceInfo->SessionPort,
        SessionListener,
        &m_sessionId,
        sessionOpts));
    alljoyn_sessionopts_destroy(sessionOpts);

    ServiceObjectPath = serviceInfo->ObjectPath;
    std::vector<char> objectPath = AllJoynHelpers::PlatformToMultibyteString(ServiceObjectPath);

    if (objectPath.empty())
    {
        return AllJoynStatus::Fail;
    }

    ProxyBusObject = alljoyn_proxybusobject_create(m_nativeBusAttachment, &sessionNameUtf8[0], &objectPath[0], m_sessionId);
    if (nullptr == ProxyBusObject)
    {
        return AllJoynStatus::Fail;
    }


    alljoyn_interfacedescription description = alljoyn_busattachment_getinterface(m_nativeBusAttachment, "org.alljoyn.Config");
    if (nullptr == description)
    {
        return AllJoynStatus::Fail;
    }

    RETURN_IF_QSTATUS_ERROR(AllJoynBusObjectManager::GetBusObject(m_nativeBusAttachment, AllJoynHelpers::PlatformToMultibyteString(ServiceObjectPath).data(), &m_busObject));
   
    if (!AllJoynBusObjectManager::BusObjectIsRegistered(m_nativeBusAttachment, m_busObject))
    {
        RETURN_IF_QSTATUS_ERROR(alljoyn_busobject_addinterface(BusObject, description));
    }


    SourceInterfaces[description] = m_weak;

    unsigned int noneMechanismIndex = 0;
    bool authenticationMechanismsContainsNone = m_busAttachment->AuthenticationMechanisms->IndexOf(AllJoynAuthenticationMechanism::None, &noneMechanismIndex);
    QCC_BOOL interfaceIsSecure = alljoyn_interfacedescription_issecure(description);

    // If the current set of AuthenticationMechanisms supports authentication, 
    // determine whether to secure the connection.
    if (AllJoynHelpers::CanSecure(m_busAttachment->AuthenticationMechanisms))
    {
        // Secure the connection if the org.alljoyn.Bus.Secure XML annotation
        // is specified, or if None is not present in AuthenticationMechanisms.
        if (!authenticationMechanismsContainsNone || interfaceIsSecure)
        {
            RETURN_IF_QSTATUS_ERROR(alljoyn_proxybusobject_secureconnection(ProxyBusObject, QCC_FALSE));
            RETURN_IF_QSTATUS_ERROR(AllJoynBusObjectManager::TryRegisterBusObject(m_nativeBusAttachment, BusObject, true));
        }
        else
        {
            RETURN_IF_QSTATUS_ERROR(AllJoynBusObjectManager::TryRegisterBusObject(m_nativeBusAttachment, BusObject, false));
        }
    }
    else
    {
        // If the current set of AuthenticationMechanisms does not support authentication
        // but the interface requires security, report an error.
        if (interfaceIsSecure)
        {
            return static_cast<int32>(ER_BUS_NO_AUTHENTICATION_MECHANISM);
        }
        else
        {
            RETURN_IF_QSTATUS_ERROR(AllJoynBusObjectManager::TryRegisterBusObject(m_nativeBusAttachment, BusObject, false));
        }
    }

    RETURN_IF_QSTATUS_ERROR(alljoyn_proxybusobject_addinterface(ProxyBusObject, description));
    
    m_signals->Initialize(BusObject, m_sessionId);

    return AllJoynStatus::Ok;
}
