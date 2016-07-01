//
// Module Name:
//
//      Adapter.cpp
//
// Abstract:
//
//      Adapter class implementation.
//
//      Adapter class implements the IAdapter interface.
//      When the DSB bridge component uses the adapter it instantiates an Adapter object 
//      and uses it as the IAdapter.
//
//
#include "pch.h"
#include "Adapter.h"
#include "AdapterDevice.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;

using namespace BridgeRT;
using namespace DsbCommon;

namespace AdapterLib
{

    // $appguidcomment$
    static const GUID APPLICATION_GUID = {0x80c3632b,0xdcf6,0x4e79,{0xa9,0xb0,0xb8,0x86,0x85,0xb3,0xd8,0xba}};

    Adapter::Adapter()
    {
        Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
        Windows::ApplicationModel::PackageId^ packageId = package->Id;
        Windows::ApplicationModel::PackageVersion versionFromPkg = packageId->Version;

        this->vendor = L"makhlu";
        this->adapterName = L"ZWMultisensorBridge";
        // the adapter prefix must be something like "com.mycompany" (only alpha num and dots)
        // it is used by the Device System Bridge as root string for all services and interfaces it exposes
        this->exposedAdapterPrefix = L"com." + DsbCommon::ToLower(this->vendor->Data());
        this->exposedApplicationGuid = Platform::Guid(APPLICATION_GUID);

        if (nullptr != package &&
            nullptr != packageId )
        {
            this->exposedApplicationName = packageId->Name;
            this->version = versionFromPkg.Major.ToString() + L"." + versionFromPkg.Minor.ToString() + L"." + versionFromPkg.Revision.ToString() + L"." + versionFromPkg.Build.ToString();
        }
        else
        {
            this->exposedApplicationName = L"DeviceSystemBridge";
            this->version = L"0.0.0.0";
        }
    }


    Adapter::~Adapter()
    {
    }


    _Use_decl_annotations_
    uint32
    Adapter::SetConfiguration(const Platform::Array<byte>^ ConfigurationData)
    {
        UNREFERENCED_PARAMETER(ConfigurationData);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::GetConfiguration(Platform::Array<byte>^* ConfigurationDataPtr)
    {
        UNREFERENCED_PARAMETER(ConfigurationDataPtr);

        return ERROR_SUCCESS;
    }


    uint32
    Adapter::Initialize()
    {
        return ERROR_SUCCESS;
    }


    uint32
    Adapter::Shutdown()
    {
        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::EnumDevices(
        ENUM_DEVICES_OPTIONS Options,
        IAdapterDeviceVector^* DeviceListPtr,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Options);
        UNREFERENCED_PARAMETER(RequestPtr);

        try
        {
            *DeviceListPtr = ref new BridgeRT::AdapterDeviceVector(this->devices);
        }
        catch (OutOfMemoryException^ ex)
        {
            throw ref new OutOfMemoryException(ex->Message);
        }

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::GetProperty(
        IAdapterProperty^ Property,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Property);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::SetProperty(
        IAdapterProperty^ Property,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Property);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::GetPropertyValue(
        IAdapterProperty^ Property,
        String^ AttributeName,
        IAdapterValue^* ValuePtr,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Property);
        UNREFERENCED_PARAMETER(AttributeName);
        UNREFERENCED_PARAMETER(ValuePtr);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::SetPropertyValue(
        IAdapterProperty^ Property,
        IAdapterValue^ Value,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Property);
        UNREFERENCED_PARAMETER(Value);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::CallMethod(
        IAdapterMethod^ Method,
        IAdapterIoRequest^* RequestPtr
        )
    {
        UNREFERENCED_PARAMETER(Method);
        UNREFERENCED_PARAMETER(RequestPtr);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::RegisterSignalListener(
        IAdapterSignal^ Signal,
        IAdapterSignalListener^ Listener,
        Object^ ListenerContext
        )
    {
        UNREFERENCED_PARAMETER(Signal);
        UNREFERENCED_PARAMETER(Listener);
        UNREFERENCED_PARAMETER(ListenerContext);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::UnregisterSignalListener(
        IAdapterSignal^ Signal,
        IAdapterSignalListener^ Listener
        )
    {
        UNREFERENCED_PARAMETER(Signal);
        UNREFERENCED_PARAMETER(Listener);

        return ERROR_SUCCESS;
    }


    _Use_decl_annotations_
    uint32
    Adapter::NotifySignalListener(
        IAdapterSignal^ Signal
        )
    {
        UNREFERENCED_PARAMETER(Signal);

        return ERROR_SUCCESS;
    }


    uint32
    Adapter::createSignals()
    {
        return ERROR_SUCCESS;
    }
} // namespace AdapterLib