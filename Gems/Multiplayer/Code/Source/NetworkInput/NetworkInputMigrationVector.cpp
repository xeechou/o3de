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

#include <Source/NetworkInput/NetworkInputMigrationVector.h>
#include <Include/INetworkEntityManager.h>
#include <AzNetworking/Serialization/ISerializer.h>

namespace Multiplayer
{
    NetworkInputMigrationVector::NetworkInputMigrationVector()
        : m_owner()
    {
        ;
    }

    NetworkInputMigrationVector::NetworkInputMigrationVector(const ConstNetworkEntityHandle& entityHandle)
        : m_owner(entityHandle)
    {
        ;
    }

    uint32_t NetworkInputMigrationVector::GetSize() const
    {
        return aznumeric_cast<uint32_t>(m_inputs.size());
    }

    NetworkInput& NetworkInputMigrationVector::operator[](uint32_t index)
    {
        return m_inputs[index].m_networkInput;
    }

    const NetworkInput& NetworkInputMigrationVector::operator[](uint32_t index) const
    {
        return m_inputs[index].m_networkInput;
    }

    bool NetworkInputMigrationVector::PushBack(const NetworkInput& networkInput)
    {
        if (m_inputs.size() < m_inputs.capacity())
        {
            m_inputs.push_back(networkInput);
            return true;
        }
        return false;
    }

    bool NetworkInputMigrationVector::Serialize(AzNetworking::ISerializer& serializer)
    {
        NetEntityId ownerId = m_owner.GetNetEntityId();
        serializer.Serialize(ownerId, "OwnerId");

        uint32_t inputCount = aznumeric_cast<uint32_t>(m_inputs.size());
        serializer.Serialize(inputCount, "InputCount");

        if (serializer.GetSerializerMode() == AzNetworking::SerializerMode::WriteToObject)
        {
            // make sure all the possible NetworkInputs get attached prior to serialization, this double sends the size, but this message is only sent on server migration
            m_inputs.resize(inputCount);
            m_owner = GetNetworkEntityManager()->GetEntity(ownerId);
            NetBindComponent* netBindComponent = m_owner.GetNetBindComponent();
            if (netBindComponent)
            {
                for (uint32_t i = 0; i < m_inputs.size(); ++i)
                {
                    m_inputs[i].m_networkInput.AttachNetBindComponent(netBindComponent);
                }
            }
        }
        return serializer.Serialize(m_inputs, "Inputs");
    }
}
