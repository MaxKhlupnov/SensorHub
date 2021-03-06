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

namespace org { namespace alljoyn { namespace About {

ref class AboutConsumer;

public ref class AboutGetAboutDataResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    property Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ AboutData
    {
        Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ get() { return m_interfaceMemberAboutData; }
    internal:
        void set(_In_ Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ value) { m_interfaceMemberAboutData = value; }
    }
    
    static AboutGetAboutDataResult^ CreateSuccessResult(_In_ Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ interfaceMemberAboutData)
    {
        auto result = ref new AboutGetAboutDataResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->AboutData = interfaceMemberAboutData;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static AboutGetAboutDataResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new AboutGetAboutDataResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
    Windows::Foundation::Collections::IMap<Platform::String^,Platform::Object^>^ m_interfaceMemberAboutData;
};

public ref class AboutGetObjectDescriptionResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    property Windows::Foundation::Collections::IVector<AboutObjectDescriptionItem^>^ ObjectDescription
    {
        Windows::Foundation::Collections::IVector<AboutObjectDescriptionItem^>^ get() { return m_interfaceMemberObjectDescription; }
    internal:
        void set(_In_ Windows::Foundation::Collections::IVector<AboutObjectDescriptionItem^>^ value) { m_interfaceMemberObjectDescription = value; }
    }
    
    static AboutGetObjectDescriptionResult^ CreateSuccessResult(_In_ Windows::Foundation::Collections::IVector<AboutObjectDescriptionItem^>^ interfaceMemberObjectDescription)
    {
        auto result = ref new AboutGetObjectDescriptionResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->ObjectDescription = interfaceMemberObjectDescription;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static AboutGetObjectDescriptionResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new AboutGetObjectDescriptionResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
    Windows::Foundation::Collections::IVector<AboutObjectDescriptionItem^>^ m_interfaceMemberObjectDescription;
};

public ref class AboutJoinSessionResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    property AboutConsumer^ Consumer
    {
        AboutConsumer^ get() { return m_consumer; }
    internal:
        void set(_In_ AboutConsumer^ value) { m_consumer = value; }
    };

private:
    int32 m_status;
    AboutConsumer^ m_consumer;
};

public ref class AboutGetVersionResult sealed
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

    static AboutGetVersionResult^ CreateSuccessResult(_In_ uint16 value)
    {
        auto result = ref new AboutGetVersionResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->Version = value;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }

    static AboutGetVersionResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new AboutGetVersionResult();
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
