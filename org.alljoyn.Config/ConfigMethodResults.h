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

using namespace concurrency;

namespace org { namespace alljoyn { namespace Config {

ref class ConfigConsumer;

public ref class ConfigFactoryResetResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    
    static ConfigFactoryResetResult^ CreateSuccessResult()
    {
        auto result = ref new ConfigFactoryResetResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static ConfigFactoryResetResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new ConfigFactoryResetResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
};

public ref class ConfigGetConfigurationsResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    property Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ Languages
    {
        Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ get() { return m_interfaceMemberLanguages; }
    internal:
        void set(_In_ Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ value) { m_interfaceMemberLanguages = value; }
    }
    
    static ConfigGetConfigurationsResult^ CreateSuccessResult(_In_ Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ interfaceMemberLanguages)
    {
        auto result = ref new ConfigGetConfigurationsResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->Languages = interfaceMemberLanguages;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static ConfigGetConfigurationsResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new ConfigGetConfigurationsResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
    Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ m_interfaceMemberLanguages;
};

public ref class ConfigResetConfigurationsResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    
    static ConfigResetConfigurationsResult^ CreateSuccessResult()
    {
        auto result = ref new ConfigResetConfigurationsResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static ConfigResetConfigurationsResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new ConfigResetConfigurationsResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
};

public ref class ConfigRestartResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    
    static ConfigRestartResult^ CreateSuccessResult()
    {
        auto result = ref new ConfigRestartResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static ConfigRestartResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new ConfigRestartResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
};

public ref class ConfigSetPasscodeResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    
    static ConfigSetPasscodeResult^ CreateSuccessResult()
    {
        auto result = ref new ConfigSetPasscodeResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static ConfigSetPasscodeResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new ConfigSetPasscodeResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
};

public ref class ConfigUpdateConfigurationsResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    
    static ConfigUpdateConfigurationsResult^ CreateSuccessResult()
    {
        auto result = ref new ConfigUpdateConfigurationsResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static ConfigUpdateConfigurationsResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new ConfigUpdateConfigurationsResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
};

public ref class ConfigJoinSessionResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    property ConfigConsumer^ Consumer
    {
        ConfigConsumer^ get() { return m_consumer; }
    internal:
        void set(_In_ ConfigConsumer^ value) { m_consumer = value; }
    };

private:
    int32 m_status;
    ConfigConsumer^ m_consumer;
};

public ref class ConfigGetVersionResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    property uint16 Version
    {
        uint16 get() { return m_value; }
    internal:
        void set(_In_ uint16 value) { m_value = value; }
    }

    static ConfigGetVersionResult^ CreateSuccessResult(_In_ uint16 value)
    {
        auto result = ref new ConfigGetVersionResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->Version = value;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }

    static ConfigGetVersionResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new ConfigGetVersionResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
    uint16 m_value;
};

} } } 
