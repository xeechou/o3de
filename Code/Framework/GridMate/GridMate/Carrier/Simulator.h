/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#ifndef GM_SIMULATOR_INTERFACE_H
#define GM_SIMULATOR_INTERFACE_H

#include <GridMate/Types.h>
#include <AzCore/std/smart_ptr/intrusive_ptr.h>

namespace GridMate
{
    class Driver;
    class DriverAddress;

    /**
     * Simulator interface
     */
    class Simulator
    {
    public:
        virtual ~Simulator() {}

        /// Called from Carrier, so simulator can use the low level driver directly.
        virtual void BindDriver(Driver* driver) = 0;
        /// Called from Carrier when driver can no longer be used(ie. will be destroyed)
        virtual void UnbindDriver() = 0;
        /// Called when Carrier has established a new connection.
        virtual void OnConnect(const AZStd::intrusive_ptr<DriverAddress>& address) = 0;
        /// Called when Carrier has lost a connection.
        virtual void OnDisconnect(const AZStd::intrusive_ptr<DriverAddress>& address) = 0;
        /// Called when Carrier has a packet to send
        virtual bool OnSend(const AZStd::intrusive_ptr<DriverAddress>& to, const void* data, unsigned int dataSize) = 0;
        /// Called when Carrier receives a packet
        virtual bool OnReceive(const AZStd::intrusive_ptr<DriverAddress>& from, const void* data, unsigned int dataSize) = 0;
        /// Called from Carrier when no more data has arrived and can supply you with data (with latency, out of order, etc).
        virtual unsigned int ReceiveDataFrom(AZStd::intrusive_ptr<DriverAddress>& from, char* data, unsigned int maxDataSize) = 0;
        ///
        virtual void Update() = 0;
    };
}

#endif // GM_SIMULATOR_INTERFACE_H

