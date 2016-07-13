// Copyright (c) 2015, Microsoft Corporation
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
// IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//

#include "pch.h"
#include "ZWaveAdapter.h"

#include "AdapterUtils.h"
#include <cctype>
#include <functional>
//openzwave
#include "Options.h"
#include "Manager.h"

#include <ppltasks.h>

using namespace Platform;

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage;
using namespace Windows::Devices::SerialCommunication;
using namespace Windows::Devices::Enumeration;
using namespace Concurrency;


using namespace OpenZWave;
using namespace std;
using namespace concurrency;
using namespace Windows::System::Threading;

namespace AdapterLib
{
   

    //
    // ZWaveAdapter class.
    // Description:
    //  The class that implements the ZWave Adapter as IAdapter.
    //
    ZWaveAdapter::ZWaveAdapter()
        : m_pMgr(nullptr)
        , m_bShutdown(false)
    {
        Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
        Windows::ApplicationModel::PackageId^ packageId = package->Id;
        Windows::ApplicationModel::PackageVersion versionFromPkg = packageId->Version;

       
    }


    ZWaveAdapter::~ZWaveAdapter()
    {
        Shutdown();
    }

   

    uint32 ZWaveAdapter::Initialize()
    {
        uint32 status = ERROR_SUCCESS;

        status = WIN32_FROM_HRESULT(m_adapterConfig.Init());
        if (status != ERROR_SUCCESS)
        {
            goto done;
        }

        m_bShutdown = false;

        //configurations
        Options::Create(ConvertTo<string>(AdapterConfig::GetConfigPath()), ConvertTo<string>(AdapterConfig::GetUserPath()), "");

        Options::Get()->AddOptionBool("Logging", false);    //Disable logging
        Options::Get()->AddOptionInt("PollInterval", 500);
        Options::Get()->AddOptionBool("IntervalBetweenPolls", true);
        Options::Get()->AddOptionBool("ConsoleOutput", false);
        Options::Get()->AddOptionBool("SaveConfiguration", false);

        Options::Get()->AddOptionString("ControllerPath", "", false);
        Options::Get()->AddOptionInt("ControllerInterface", (int)(Driver::ControllerInterface_Serial));
        Options::Get()->AddOptionInt("NetworkMonitorInterval", 30000);  //30 seconds


        Options::Get()->Lock();

        //instantiate the Manager object
        m_pMgr = Manager::Create();

        //add awatcher for notification
        m_pMgr->AddWatcher(OnNotification, reinterpret_cast<void*>(this));


        //Get the network monitor interval
        int32 nInterval;
        Options::Get()->GetOptionAsInt("NetworkMonitorInterval", &nInterval);
        m_networkMonitorTimeout = nInterval * 10000; //in 100 nano second interval

        StartDeviceDiscovery();

    done:
        return status;
    }


	NodeInfo^ ZWaveAdapter::GetNodeInfo(uint32 homeId, uint8 nodeId) {
		NodeInfo^ info = ref new NodeInfo();
		
		info->Type =ToPlatformString(m_pMgr->GetNodeType(homeId, nodeId));
		info->LibraryVersion = ToPlatformString(m_pMgr->GetLibraryVersion(homeId));
		info->Manufacturer = ToPlatformString(m_pMgr->GetNodeManufacturerName(homeId, nodeId));
		info->Location = ToPlatformString(m_pMgr->GetNodeLocation(homeId, nodeId));
		info->Name = ToPlatformString(m_pMgr->GetNodeName(homeId, nodeId));
		info->Product = ToPlatformString(m_pMgr->GetNodeProductName(homeId, nodeId));

		return info;
	}

    uint32 ZWaveAdapter::Shutdown()
    {
        uint32 status = ERROR_SUCCESS;

        m_bShutdown = true;

        if (m_DiscoveryTimer)
        {
            m_DiscoveryTimer->Cancel();
            m_DiscoveryTimer = nullptr;
        }

        if (m_MonitorTimer)
        {
            m_MonitorTimer->Cancel();
            m_MonitorTimer = nullptr;
        }

        Manager::Destroy();
        m_pMgr = nullptr;

        Options::Destroy();

        return status;
    }



    void ZWaveAdapter::StartDeviceDiscovery()
    {
        string path;
        int inf = static_cast<int>(Driver::ControllerInterface_Serial);

        if (Options::Get())
        {
            Options::Get()->GetOptionAsString("ControllerPath", &path);
            Options::Get()->GetOptionAsInt("ControllerInterface", &inf);

            auto selector = SerialDevice::GetDeviceSelector();

            create_task(DeviceInformation::FindAllAsync(selector))
                .then([path, inf, this](DeviceInformationCollection ^ devices)
            {
				for (auto iterator = devices->First(); iterator->HasCurrent; iterator->MoveNext())
				{
					wstring wCurrentId = iterator->Current->Id->Data();
					string currentId = ConvertTo<string>(wCurrentId);
					if (currentId.find(path) != string::npos && m_pMgr)
					{
						m_pMgr->AddDriver(currentId, (Driver::ControllerInterface)inf);
					}
				}
            });
        }

        //Set the time for next discovery
        TimeSpan ts;
        ts.Duration = m_networkMonitorTimeout;
        m_DiscoveryTimer = ThreadPoolTimer::CreateTimer(ref new TimerElapsedHandler([this](ThreadPoolTimer^ timer)
        {
            StartDeviceDiscovery();
        }), ts);
    }

   

	
		// ---------------------------------------------------------------------------- -
		//	Notifications
		//-----------------------------------------------------------------------------


  

    void ZWaveAdapter::OnNotification(Notification const* _notification, void* _context)
    {
        ZWaveAdapter^ adapter = reinterpret_cast<ZWaveAdapter^>(_context);

        uint32 const homeId = _notification->GetHomeId();
        uint8 const nodeId = _notification->GetNodeId();
		
		ZWNotification^ notification = ref new ZWNotification(_notification);
		adapter->ZWaveNotification(notification);
		

        try
        {
            switch (_notification->GetType())
            {
            case Notification::Type_Notification:
            {
                uint8 notifCode = _notification->GetNotification();
                switch (notifCode)
                {
                case Notification::NotificationCode::Code_Dead:
                {
                    //The device is dead, move the device to pending devices
                   // adapter->RemoveDevice(nodeId, true);
                    break;
                }
                case Notification::NotificationCode::Code_Alive:
                {
                    //if we had already received the info before, move the device to m_devices
                    //and notify Device arrival.
                    //If the info has not been received yet, leave it in m_pendindDevices as more info will follow.
                    if (Manager::Get()->IsNodeInfoReceived(homeId, nodeId))
                    {
                     //   adapter->AddDevice(homeId, nodeId, false);
                    }

                    break;
                }
                }   //switch
                break;
            }
            case Notification::Type_DriverReady:
            {
                //Start the DeviceMonitor timer
                TimeSpan ts;
                ts.Duration = adapter->m_networkMonitorTimeout;
                adapter->m_MonitorTimer = ThreadPoolTimer::CreatePeriodicTimer(ref new TimerElapsedHandler([homeId](ThreadPoolTimer^ timer)
                {
                    if (Manager::Get())
                    {
                       Manager::Get()->TestNetwork(homeId, 1);
                    }
                }), ts);

                break;
            }
            case Notification::Type_DriverRemoved:
            case Notification::Type_DriverFailed:
            {
                if (adapter->m_MonitorTimer)
                {
                    adapter->m_MonitorTimer->Cancel();
                    adapter->m_MonitorTimer = nullptr;
                }
               // adapter->RemoveAllDevices(homeId);

                if ((_notification->GetType() == Notification::Type_DriverFailed) && !(adapter->m_bShutdown))
                {
                    //remove the driver
                    if (homeId != 0)
                    {
                        create_task([homeId]()
                        {
                            Manager::Get()->RemoveDriver(Manager::Get()->GetControllerPath(homeId));
                        });
                    }
                }

                break;
            }
            case Notification::Type_NodeAdded:
            {
                // Add the new node to our pending device list as we dont yet have all the values for the node
               // adapter->AddDevice(homeId, nodeId, true);
                break;
            }
            case Notification::Type_NodeRemoved:
            {
                // Remove the node from our list
               // adapter->RemoveDevice(homeId, nodeId);
                break;
            }
            case Notification::Type_NodeQueriesComplete:
            {
                //move the device from the pending list to actual list and notify the signal
               // adapter->AddDevice(homeId, nodeId, false);
                break;
            }
            case Notification::Type_ValueAdded:
            {	
				// Add the new node to our list 
				  
				  uint32 m_homeId = _notification->GetHomeId();
				  uint32 m_nodeId = _notification->GetNodeId();


           /*     //Value added should be received only during the addition of node. Hence look in the pending device list
                auto iter = adapter->FindDevice(adapter->m_pendingDevices, homeId, nodeId);
                if (iter != adapter->m_pendingDevices.end())
                {
                    //add the value
                    ValueID value = _notification->GetValueID();
                //    dynamic_cast<ZWaveAdapterDevice^>(*iter)->AddPropertyValue(value);
                }
				std::string * sensor_multilevel = new std::string("COMMAND_CLASS_SENSOR_MULTILEVEL");
				
				uint16 DeviceType = adapter->m_pMgr->GetNodeDeviceType(homeId, nodeId);
				bool isSensor = adapter->m_pMgr->GetNodeClassInformation(homeId, nodeId, SensorMultilevel::StaticGetCommandClassId(), sensor_multilevel, (uint8 *)5);

				// If this device is switch 0 listen 
				if (isSensor) {
					
				}
				*/
                break;
            }
            case Notification::Type_ValueRemoved:
            {
                //Value removed should be received only during the addition of node. Hence look in the pending device list
              /*  auto iter = adapter->FindDevice(adapter->m_pendingDevices, homeId, nodeId);
                if (iter != adapter->m_pendingDevices.end())
                {
                    //remove the value
                    dynamic_cast<ZWaveAdapterDevice^>(*iter)->RemovePropertyValue(_notification->GetValueID());
                }
                break;*/
            }
            case Notification::Type_ValueChanged:
            {
                //look into device list
           /*     auto iter = adapter->FindDevice(adapter->m_devices, homeId, nodeId);
                if (iter != adapter->m_devices.end())
                {
                    ZWaveAdapterDevice^ device = dynamic_cast<ZWaveAdapterDevice^>(*iter);

                    //add the value
                    device->UpdatePropertyValue(_notification->GetValueID());

                    //notify the signal
                    AutoLock sync(adapter->m_signalLock);

                    IAdapterSignal^ signal = device->GetSignal(Constants::CHANGE_OF_VALUE_SIGNAL);
                    if (signal != nullptr)
                    {
                        ZWaveAdapterProperty^ adapterProperty = nullptr;
                        ZWaveAdapterValue^ adapterValue = nullptr;

                        auto adapterPropertyIter = device->GetProperty(_notification->GetValueID());

                        if (adapterPropertyIter != device->m_properties.end())
                        {
                            adapterProperty = dynamic_cast<ZWaveAdapterProperty^>(*adapterPropertyIter);

                            //get the AdapterValue
                            adapterValue = adapterProperty->GetAttributeByName(ref new String(ValueName.c_str()));

                            // Set the 'Property_Handle' signal parameter
                            signal->Params->GetAt(0)->Data = adapterProperty;

                            // Set the 'Property_Handle' signal parameter
                            signal->Params->GetAt(1)->Data = adapterValue;

                            adapter->NotifySignalListener(signal);
                        }
                    }
                }*/
                break;
            }
            default:
                break;
            }
        }
        catch (Exception^)
        {
            //just ignore
        }
    }

} // namespace AdapterLib
