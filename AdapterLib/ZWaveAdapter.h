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

#pragma once


#include "AdapterConfig.h"

#include "Manager.h"
#include "Notification.h"

#include <string>
#include <map>

namespace AdapterLib
{
   
    //
    // ZWaveAdapter class.
    // Description:
    //  The class that implements the ZWave Adapter as IAdapter.
    //
    public ref class ZWaveAdapter sealed        
    {
    public:



        uint32 Initialize();
        uint32 Shutdown();

        //
        // Construction
        //
        ZWaveAdapter();
        virtual ~ZWaveAdapter();

    internal:
        static void OnNotification(OpenZWave::Notification const * _notification, void * _context);

    private:
       
        void StartDeviceDiscovery();
       /* void AddDevice(const uint32 homeId, const uint8 nodeId, bool bPending);
        void RemoveDevice(const uint32 homeId, const uint8 nodeId, bool bMoveToPending = false);
        void RemoveAllDevices(uint32 homeId);*/

    private:

        

        // Sync object
        std::recursive_mutex m_configLock;
        std::recursive_mutex m_deviceListLock;
        std::recursive_mutex m_signalLock;

        //Adapter Config
        AdapterConfig m_adapterConfig;

        //
     

        OpenZWave::Manager *m_pMgr;
        Windows::System::Threading::ThreadPoolTimer^ m_MonitorTimer{ nullptr };
        Windows::System::Threading::ThreadPoolTimer^ m_DiscoveryTimer{ nullptr };

        bool m_bShutdown;

       int64 m_networkMonitorTimeout;
    };

} // namespace AdapterLib
