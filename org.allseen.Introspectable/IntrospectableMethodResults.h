//-----------------------------------------------------------------------------
// <auto-generated>
//   This code was generated by a tool.
//
//   Changes to this file may cause incorrect behavior and will be lost if
//   the code is regenerated.
//
//   For more information, see: http://go.microsoft.com/fwlink/?LinkID=623246
// </auto-generated>
//-----------------------------------------------------------------------------
#pragma once

using namespace concurrency;

namespace org { namespace allseen { namespace Introspectable {

ref class IntrospectableConsumer;

public ref class IntrospectableGetDescriptionLanguagesResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    property Windows::Foundation::Collections::IVector<Platform::String^>^ LanguageTags
    {
        Windows::Foundation::Collections::IVector<Platform::String^>^ get() { return m_interfaceMemberLanguageTags; }
    internal:
        void set(_In_ Windows::Foundation::Collections::IVector<Platform::String^>^ value) { m_interfaceMemberLanguageTags = value; }
    }
    
    static IntrospectableGetDescriptionLanguagesResult^ CreateSuccessResult(_In_ Windows::Foundation::Collections::IVector<Platform::String^>^ interfaceMemberLanguageTags)
    {
        auto result = ref new IntrospectableGetDescriptionLanguagesResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->LanguageTags = interfaceMemberLanguageTags;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static IntrospectableGetDescriptionLanguagesResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new IntrospectableGetDescriptionLanguagesResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
    Windows::Foundation::Collections::IVector<Platform::String^>^ m_interfaceMemberLanguageTags;
};

public ref class IntrospectableIntrospectWithDescriptionResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    property Platform::String^ Data
    {
        Platform::String^ get() { return m_interfaceMemberData; }
    internal:
        void set(_In_ Platform::String^ value) { m_interfaceMemberData = value; }
    }
    
    static IntrospectableIntrospectWithDescriptionResult^ CreateSuccessResult(_In_ Platform::String^ interfaceMemberData)
    {
        auto result = ref new IntrospectableIntrospectWithDescriptionResult();
        result->Status = Windows::Devices::AllJoyn::AllJoynStatus::Ok;
        result->Data = interfaceMemberData;
        result->m_creationContext = Concurrency::task_continuation_context::use_current();
        return result;
    }
    
    static IntrospectableIntrospectWithDescriptionResult^ CreateFailureResult(_In_ int32 status)
    {
        auto result = ref new IntrospectableIntrospectWithDescriptionResult();
        result->Status = status;
        return result;
    }
internal:
    Concurrency::task_continuation_context m_creationContext = Concurrency::task_continuation_context::use_default();

private:
    int32 m_status;
    Platform::String^ m_interfaceMemberData;
};

public ref class IntrospectableJoinSessionResult sealed
{
public:
    property int32 Status
    {
        int32 get() { return m_status; }
    internal:
        void set(_In_ int32 value) { m_status = value; }
    }

    property IntrospectableConsumer^ Consumer
    {
        IntrospectableConsumer^ get() { return m_consumer; }
    internal:
        void set(_In_ IntrospectableConsumer^ value) { m_consumer = value; }
    };

private:
    int32 m_status;
    IntrospectableConsumer^ m_consumer;
};

} } } 
