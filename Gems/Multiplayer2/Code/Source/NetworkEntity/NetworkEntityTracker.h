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

#pragma once

#include <Source/MultiplayerTypes.h>
#include <Source/NetworkEntity/NetworkEntityHandle.h>
#include <AzCore/std/containers/unordered_map.h>
#include <AzCore/Component/Entity.h>

namespace Multiplayer
{
    //! @class NetworkEntityTracker
    //! @brief The responsibly of this class is to allow entity netEntityId's to be looked up.
    class NetworkEntityTracker
    {
    public:

        using EntityMap = AZStd::unordered_map<NetEntityId, AZ::Entity*>;
        using iterator = EntityMap::iterator;
        using const_iterator = EntityMap::const_iterator;

        NetworkEntityTracker() = default;

        //! Adds a networked entity to the tracker
        //! @param netEntityId the networkId of the entity to add
        //! @param entity pointer to the entity corresponding to the networkId
        void Add(NetEntityId netEntityId, AZ::Entity* entity);

        //! Returns an entity handle which can validate entity existence.
        NetworkEntityHandle Get(NetEntityId netEntityId);
        ConstNetworkEntityHandle Get(NetEntityId netEntityId) const;

        //! Returns true if the netEntityId exists.
        bool Exists(NetEntityId netEntityId) const;

        //! Get a raw pointer of an entity.
        AZ::Entity *GetRaw(NetEntityId netEntityId) const;

        //! Moves the given iterator out of the entity holder and returns the ptr
        AZ::Entity *Move(EntityMap::iterator iter);

        //! Container overloads
        //!@{
        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
        iterator find(NetEntityId netEntityId);
        const_iterator find(NetEntityId netEntityId) const;
        void erase(NetEntityId netEntityId);
        iterator erase(EntityMap::iterator iter);
        AZStd::size_t size() const;
        void clear();
        //! @}

        //! Dirty tracking optimizations to avoid unnecessary hash lookups.
        //! There are two counts, one for adds and one for deletes
        //! If an entity is nullptr, check adds to check to see if our entity was added again
        //! If an entity is not nullptr, check removes which reminds us to see if the entity no longer exists
        //! Passing in the entity into this helper assists in retrieving the correct count, so we do not need to store both counts inside each handle
        uint32_t GetChangeDirty(const AZ::Entity* entity) const;
        uint32_t GetDeleteChangeDirty() const;
        uint32_t GetAddChangeDirty() const;

        //! Prevent copying and heap allocation.
        AZ_DISABLE_COPY_MOVE(NetworkEntityTracker);

    private:

        EntityMap m_entityMap;
        uint32_t m_deleteChangeDirty = 0;
        uint32_t m_addChangeDirty = 0;
    };
}

#include "Source/NetworkEntity/NetworkEntityTracker.inl"
