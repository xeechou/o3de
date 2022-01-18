/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/IO/GenericStreams.h>
#include <AzCore/std/smart_ptr/unique_ptr.h>
#include <AzCore/std/string/string.h>

#include <AzToolsFramework/Prefab/Instance/Instance.h>
#include <AzToolsFramework/Prefab/Spawnable/InMemorySpawnableAssetContainer.h>

namespace AzToolsFramework
{

    class PrefabEditorEntityOwnershipInterface
    {
    public:
        AZ_RTTI(PrefabEditorEntityOwnershipInterface,"{38E764BA-A089-49F3-848F-46018822CE2E}");

        //! Creates a prefab instance with the provided entities and nestedPrefabInstances.
        //! /param entities The entities to put under the new prefab.
        //! /param nestedPrefabInstances The nested prefab instances to put under the new prefab.
        //! /param filePath The filepath corresponding to the prefab file to be created.
        //! /param instanceToParentUnder The instance the newly created prefab instance is parented under.
        //! /return The optional reference to the prefab created.
        virtual Prefab::InstanceOptionalReference CreatePrefab(
            const AZStd::vector<AZ::Entity*>& entities, AZStd::vector<AZStd::unique_ptr<Prefab::Instance>>&& nestedPrefabInstances,
            AZ::IO::PathView filePath, Prefab::InstanceOptionalReference instanceToParentUnder = AZStd::nullopt) = 0;

        //! Instantiate the prefab file provided.
        //! /param filePath The filepath for the prefab file the instance should be created from.
        //! /param instanceToParentUnder The instance the newly instantiated prefab instance is parented under.
        //! /return The optional reference to the prefab instance.
        virtual Prefab::InstanceOptionalReference InstantiatePrefab(
            AZ::IO::PathView filePath, Prefab::InstanceOptionalReference instanceToParentUnder = AZStd::nullopt) = 0;

        virtual Prefab::InstanceOptionalReference GetRootPrefabInstance() = 0;

        virtual Prefab::TemplateId GetRootPrefabTemplateId() = 0;

        //! Get all Assets generated by Prefab processing when entering Play-In Editor mode (Ctrl+G)
        //! /return The vector of Assets generated by Prefab processing
        virtual const Prefab::PrefabConversionUtils::InMemorySpawnableAssetContainer::SpawnableAssets& GetPlayInEditorAssetData() const = 0;

        virtual bool LoadFromStream(AZ::IO::GenericStream& stream, AZStd::string_view filename) = 0;
        virtual bool SaveToStream(AZ::IO::GenericStream& stream, AZStd::string_view filename) = 0;
        
        virtual void StartPlayInEditor() = 0;
        virtual void StopPlayInEditor() = 0;

        virtual void CreateNewLevelPrefab(AZStd::string_view filename, const AZStd::string& templateFilename) = 0;

        virtual bool IsRootPrefabAssigned() const = 0;
    };
}
