/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzToolsFramework/Metadata/UuidUtils.h>
#include <AzToolsFramework/Metadata/MetadataManager.h>
#include <AzToolsFramework/Metadata/UuidEntry.h>

namespace AzToolsFramework
{
    void UuidUtilComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<UuidUtilComponent, AZ::Component>();
        }
    }

    bool UuidUtilComponent::CreateSourceUuid(AZ::IO::PathView absoluteFilePath, AZ::Uuid uuid)
    {
        auto* metadataInterface = AZ::Interface<IMetadataRequests>::Get();

        if (!metadataInterface)
        {
            AZ_Assert(metadataInterface, "Programmer Error - IMetadataRequests interface is not available");
            return false;
        }

        AzToolsFramework::UuidEntry entry;

        if (metadataInterface->GetValue(absoluteFilePath, UuidKey, entry) && !entry.m_uuid.IsNull())
        {
            AZ_Error(
                "UuidUtil",
                false,
                "File `" AZ_STRING_FORMAT "` already has a UUID assigned (" AZ_STRING_FORMAT ").  New UUID (" AZ_STRING_FORMAT ") will not be assigned.",
                AZ_STRING_ARG(absoluteFilePath.Native()),
                AZ_STRING_ARG(entry.m_uuid.ToFixedString()),
                AZ_STRING_ARG(uuid.ToFixedString()));
            return false;
        }

        entry.m_uuid = uuid;

        return metadataInterface->SetValue(absoluteFilePath, UuidKey, entry);
    }

    AZ::Uuid UuidUtilComponent::CreateSourceUuid(AZ::IO::PathView absoluteFilePath)
    {
        auto uuid = AZ::Uuid::CreateRandom();

        if(CreateSourceUuid(absoluteFilePath, uuid))
        {
            return uuid;
        }

        return {};
    }
} // namespace AzToolsFramework
