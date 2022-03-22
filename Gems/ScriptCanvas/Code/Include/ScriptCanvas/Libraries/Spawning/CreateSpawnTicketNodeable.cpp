/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzFramework/Spawnable/SpawnableBus.h>
#include <ScriptCanvas/Libraries/Spawning/CreateSpawnTicketNodeable.h>

namespace ScriptCanvas::Nodeables::Spawning
{
    AzFramework::EntitySpawnTicket CreateSpawnTicketNodeable::CreateTicket(const AzFramework::SpawnableAssetRef& Prefab)
    {
        using namespace AzFramework;

        EntitySpawnTicket spawnTicket;
        SpawnableRequestsBus::BroadcastResult(spawnTicket, &SpawnableRequests::CreateSpawnTicket, Prefab);
        return spawnTicket;
    }
}
