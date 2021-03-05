#
# All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
# its licensors.
#
# For complete copyright and license terms please see the LICENSE at the root of this
# distribution (the "License"). All use of this software is governed by the License,
# or, if provided, by the license below or the license accompanying this file. Do not
# remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#

set(FILES
    Source/StdAfx.cpp
    Source/StdAfx.h
    Include/Blast/BlastActor.h
    Include/Blast/BlastActorData.h
    Include/Blast/BlastFamilyComponentBus.h
    Include/Blast/BlastMaterial.h
    Include/Blast/BlastSystemBus.h
    Include/Blast/BlastDebug.h
    Source/Actor/BlastActorDesc.h
    Source/Actor/BlastActorFactory.h
    Source/Actor/BlastActorFactory.cpp
    Source/Actor/BlastActorImpl.h
    Source/Actor/BlastActorImpl.cpp
    Source/Actor/EntityProvider.h
    Source/Actor/EntityProvider.cpp
    Source/Actor/ShapesProvider.h
    Source/Actor/ShapesProvider.cpp
    Source/Asset/BlastAsset.h
    Source/Asset/BlastAsset.cpp
    Source/Asset/BlastAssetHandler.h
    Source/Asset/BlastAssetHandler.cpp
    Source/Asset/BlastSliceAsset.h
    Source/Asset/BlastSliceAsset.cpp
    Source/Components/BlastFamilyComponent.h
    Source/Components/BlastFamilyComponent.cpp
    Source/Components/BlastFamilyComponentNotificationBusHandler.h
    Source/Components/BlastFamilyComponentNotificationBusHandler.cpp
    Source/Components/BlastMeshDataComponent.h
    Source/Components/BlastMeshDataComponent.cpp
    Source/Components/BlastSystemComponent.cpp
    Source/Components/BlastSystemComponent.h
    Source/Family/ActorRenderManager.h
    Source/Family/ActorRenderManager.cpp
    Source/Family/ActorTracker.h
    Source/Family/ActorTracker.cpp
    Source/Family/BlastFamily.h
    Source/Family/BlastFamilyImpl.h
    Source/Family/BlastFamilyImpl.cpp
    Source/Family/DamageManager.h
    Source/Family/DamageManager.cpp
    Source/Common/BlastInterfaces.h
    Source/Common/BlastMaterial.cpp
    Source/Common/Utils.h
)
