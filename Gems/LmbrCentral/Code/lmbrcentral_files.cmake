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
    Source/LmbrCentral_precompiled.cpp
    Source/LmbrCentral_precompiled.h
    include/LmbrCentral/Ai/NavigationComponentBus.h
    include/LmbrCentral/Ai/NavigationAreaBus.h
    include/LmbrCentral/Ai/NavigationSystemBus.h
    include/LmbrCentral/Ai/NavigationSeedBus.h
    include/LmbrCentral/Animation/AttachmentComponentBus.h
    include/LmbrCentral/Audio/AudioEnvironmentComponentBus.h
    include/LmbrCentral/Audio/AudioListenerComponentBus.h
    include/LmbrCentral/Audio/AudioMultiPositionComponentBus.h
    include/LmbrCentral/Audio/AudioPreloadComponentBus.h
    include/LmbrCentral/Audio/AudioProxyComponentBus.h
    include/LmbrCentral/Audio/AudioRtpcComponentBus.h
    include/LmbrCentral/Audio/AudioSwitchComponentBus.h
    include/LmbrCentral/Audio/AudioSystemComponentBus.h
    include/LmbrCentral/Audio/AudioTriggerComponentBus.h
    include/LmbrCentral/Bundling/BundlingSystemComponentBus.h
    include/LmbrCentral/Geometry/GeometrySystemComponentBus.h
    include/LmbrCentral/Dependency/DependencyMonitor.h
    include/LmbrCentral/Dependency/DependencyMonitor.inl
    include/LmbrCentral/Dependency/DependencyNotificationBus.h
    include/LmbrCentral/Physics/WindVolumeRequestBus.h
    include/LmbrCentral/Physics/ForceVolumeRequestBus.h
    include/LmbrCentral/Physics/WaterNotificationBus.h
    include/LmbrCentral/Rendering/DecalComponentBus.h
    include/LmbrCentral/Rendering/LensFlareAsset.h
    include/LmbrCentral/Rendering/LensFlareComponentBus.h
    include/LmbrCentral/Rendering/LightComponentBus.h
    include/LmbrCentral/Rendering/MaterialAsset.h
    include/LmbrCentral/Rendering/MaterialHandle.h
    include/LmbrCentral/Rendering/MaterialOwnerBus.h
    include/LmbrCentral/Rendering/MeshAsset.h
    include/LmbrCentral/Rendering/MeshComponentBus.h
    include/LmbrCentral/Rendering/MeshModificationBus.h
    include/LmbrCentral/Rendering/RenderNodeBus.h
    include/LmbrCentral/Rendering/FogVolumeComponentBus.h
    include/LmbrCentral/Rendering/HighQualityShadowComponentBus.h
    include/LmbrCentral/Rendering/GeomCacheComponentBus.h
    include/LmbrCentral/Rendering/GiRegistrationBus.h
    include/LmbrCentral/Rendering/RenderBoundsBus.h
    include/LmbrCentral/Rendering/Utils/MaterialOwnerRequestBusHandlerImpl.h
    include/LmbrCentral/Scripting/EditorTagComponentBus.h
    include/LmbrCentral/Scripting/GameplayNotificationBus.h
    include/LmbrCentral/Scripting/SimpleStateComponentBus.h
    include/LmbrCentral/Scripting/SpawnerComponentBus.h
    include/LmbrCentral/Scripting/RandomTimedSpawnerComponentBus.h
    include/LmbrCentral/Scripting/TagComponentBus.h
    include/LmbrCentral/Shape/EditorShapeComponentBus.h
    include/LmbrCentral/Shape/ShapeComponentBus.h
    include/LmbrCentral/Shape/SphereShapeComponentBus.h
    include/LmbrCentral/Shape/BoxShapeComponentBus.h
    include/LmbrCentral/Shape/CylinderShapeComponentBus.h
    include/LmbrCentral/Shape/CapsuleShapeComponentBus.h
    include/LmbrCentral/Shape/DiskShapeComponentBus.h
    include/LmbrCentral/Shape/CompoundShapeComponentBus.h
    include/LmbrCentral/Shape/QuadShapeComponentBus.h
    include/LmbrCentral/Shape/SplineComponentBus.h
    include/LmbrCentral/Shape/PolygonPrismShapeComponentBus.h
    include/LmbrCentral/Shape/TubeShapeComponentBus.h
    include/LmbrCentral/Shape/SplineAttribute.h
    include/LmbrCentral/Shape/SplineAttribute.inl
    include/LmbrCentral/Terrain/TerrainSystemRequestBus.h
    Source/Ai/NavigationSystemComponent.h
    Source/Ai/NavigationSystemComponent.cpp
    Source/Animation/AttachmentComponent.h
    Source/Animation/AttachmentComponent.cpp
    Source/Audio/AudioAreaEnvironmentComponent.h
    Source/Audio/AudioAreaEnvironmentComponent.cpp
    Source/Audio/AudioEnvironmentComponent.h
    Source/Audio/AudioEnvironmentComponent.cpp
    Source/Audio/AudioListenerComponent.h
    Source/Audio/AudioListenerComponent.cpp
    Source/Audio/AudioMultiPositionComponent.h
    Source/Audio/AudioMultiPositionComponent.cpp
    Source/Audio/AudioPreloadComponent.h
    Source/Audio/AudioPreloadComponent.cpp
    Source/Audio/AudioProxyComponent.h
    Source/Audio/AudioProxyComponent.cpp
    Source/Audio/AudioRtpcComponent.h
    Source/Audio/AudioRtpcComponent.cpp
    Source/Audio/AudioSwitchComponent.h
    Source/Audio/AudioSwitchComponent.cpp
    Source/Audio/AudioSystemComponent.h
    Source/Audio/AudioSystemComponent.cpp
    Source/Audio/AudioTriggerComponent.h
    Source/Audio/AudioTriggerComponent.cpp
    Source/Asset/AssetSystemDebugComponent.h
    Source/Asset/AssetSystemDebugComponent.cpp
    Source/Bundling/BundlingSystemComponent.h
    Source/Bundling/BundlingSystemComponent.cpp
    Source/Events/ReflectScriptableEvents.h
    Source/Events/ReflectScriptableEvents.cpp
    Source/Geometry/GeometrySystemComponent.h
    Source/Geometry/GeometrySystemComponent.cpp
    Source/Rendering/DecalComponent.h
    Source/Rendering/DecalComponent.cpp
    Source/Rendering/LensFlareAssetHandler.h
    Source/Rendering/LensFlareAssetHandler.cpp
    Source/Rendering/LensFlareComponent.h
    Source/Rendering/LensFlareComponent.cpp
    Source/Rendering/LightComponent.h
    Source/Rendering/LightComponent.cpp
    Source/Rendering/LightInstance.h
    Source/Rendering/LightInstance.cpp
    Source/Rendering/MaterialHandle.cpp
    Source/Rendering/FogVolumeComponent.h
    Source/Rendering/FogVolumeComponent.cpp
    Source/Rendering/FogVolumeCommon.h
    Source/Rendering/FogVolumeCommon.cpp
    Source/Rendering/FogVolumeRequestsHandler.h
    Source/Rendering/FogVolumeRequestsHandler.cpp
    Source/Rendering/MeshAssetHandler.h
    Source/Rendering/MeshAssetHandler.cpp
    Source/Rendering/MeshComponent.h
    Source/Rendering/MeshComponent.cpp
    Source/Rendering/SkinnedMeshComponent.h
    Source/Rendering/SkinnedMeshComponent.cpp
    Source/Rendering/StereoRendererComponent.h
    Source/Rendering/StereoRendererComponent.cpp
    Source/Rendering/HighQualityShadowComponent.h
    Source/Rendering/HighQualityShadowComponent.cpp
    Source/Rendering/GeomCacheComponent.h
    Source/Rendering/GeomCacheComponent.cpp
    Source/Rendering/EntityDebugDisplayComponent.h
    Source/Rendering/EntityDebugDisplayComponent.cpp
    Source/Scripting/LookAtComponent.h
    Source/Scripting/LookAtComponent.cpp
    Source/Scripting/RandomTimedSpawnerComponent.cpp
    Source/Scripting/RandomTimedSpawnerComponent.h
    Source/Scripting/SimpleStateComponent.h
    Source/Scripting/SimpleStateComponent.cpp
    Source/Scripting/TagComponent.h
    Source/Scripting/TagComponent.cpp
    Source/Shape/ShapeDisplay.h
    Source/Shape/ShapeComponent.cpp
    Source/Shape/SphereShape.h
    Source/Shape/SphereShape.cpp
    Source/Shape/SphereShapeComponent.h
    Source/Shape/SphereShapeComponent.cpp
    Source/Shape/BoxShape.h
    Source/Shape/BoxShape.cpp
    Source/Shape/BoxShapeComponent.h
    Source/Shape/BoxShapeComponent.cpp
    Source/Shape/CylinderShape.h
    Source/Shape/CylinderShape.cpp
    Source/Shape/CylinderShapeComponent.h
    Source/Shape/CylinderShapeComponent.cpp
    Source/Shape/CapsuleShape.h
    Source/Shape/CapsuleShape.cpp
    Source/Shape/CapsuleShapeComponent.h
    Source/Shape/CapsuleShapeComponent.cpp
    Source/Shape/DiskShape.h
    Source/Shape/DiskShape.cpp
    Source/Shape/DiskShapeComponent.h
    Source/Shape/DiskShapeComponent.cpp
    Source/Shape/CompoundShapeComponent.h
    Source/Shape/CompoundShapeComponent.cpp
    Source/Shape/QuadShape.h
    Source/Shape/QuadShape.cpp
    Source/Shape/QuadShapeComponent.h
    Source/Shape/QuadShapeComponent.cpp
    Source/Shape/SplineComponent.h
    Source/Shape/SplineComponent.cpp
    Source/Shape/PolygonPrismShape.h
    Source/Shape/PolygonPrismShape.cpp
    Source/Shape/PolygonPrismShapeComponent.h
    Source/Shape/PolygonPrismShapeComponent.cpp
    Source/Shape/TubeShapeComponent.h
    Source/Shape/TubeShapeComponent.cpp
    Source/Shape/ShapeComponentConverters.h
    Source/Shape/ShapeComponentConverters.cpp
    Source/Shape/ShapeComponentConverters.inl
    Source/Shape/ShapeGeometryUtil.h
    Source/Shape/ShapeGeometryUtil.cpp
    Source/Unhandled/Animation/AnimationEventsAssetTypeInfo.cpp
    Source/Unhandled/Animation/AnimationEventsAssetTypeInfo.h
    Source/Unhandled/Animation/DbaAssetTypeInfo.cpp
    Source/Unhandled/Animation/DbaAssetTypeInfo.h
    Source/Unhandled/Material/MaterialAssetTypeInfo.cpp
    Source/Unhandled/Material/MaterialAssetTypeInfo.h
    Source/Unhandled/Other/AudioAssetTypeInfo.cpp
    Source/Unhandled/Other/AudioAssetTypeInfo.h
    Source/Unhandled/Other/CharacterPhysicsAssetTypeInfo.cpp
    Source/Unhandled/Other/CharacterPhysicsAssetTypeInfo.h
    Source/Unhandled/Other/CharacterRigAssetTypeInfo.cpp
    Source/Unhandled/Other/CharacterRigAssetTypeInfo.h
    Source/Unhandled/Other/EntityPrototypeLibraryAssetTypeInfo.cpp
    Source/Unhandled/Other/EntityPrototypeLibraryAssetTypeInfo.h
    Source/Unhandled/Other/GameTokenAssetTypeInfo.cpp
    Source/Unhandled/Other/GameTokenAssetTypeInfo.h
    Source/Unhandled/Other/GroupAssetTypeInfo.cpp
    Source/Unhandled/Other/GroupAssetTypeInfo.h
    Source/Unhandled/Other/PrefabsLibraryAssetTypeInfo.cpp
    Source/Unhandled/Other/PrefabsLibraryAssetTypeInfo.h
    Source/Unhandled/Other/SkeletonAssetTypeInfo.cpp
    Source/Unhandled/Other/SkeletonAssetTypeInfo.h
    Source/Unhandled/Other/SkeletonParamsAssetTypeInfo.cpp
    Source/Unhandled/Other/SkeletonParamsAssetTypeInfo.h
    Source/Unhandled/Other/SkinnedMeshAssetTypeInfo.cpp
    Source/Unhandled/Other/SkinnedMeshAssetTypeInfo.h
    Source/Unhandled/Texture/SubstanceAssetTypeInfo.cpp
    Source/Unhandled/Texture/SubstanceAssetTypeInfo.h
    Source/Unhandled/Texture/TextureAssetTypeInfo.cpp
    Source/Unhandled/Texture/TextureAssetTypeInfo.h
    Source/Unhandled/Hidden/TextureMipmapAssetTypeInfo.cpp
    Source/Unhandled/Hidden/TextureMipmapAssetTypeInfo.h
    Source/Unhandled/UI/EntityIconAssetTypeInfo.cpp
    Source/Unhandled/UI/EntityIconAssetTypeInfo.h
    Source/Unhandled/UI/FontAssetTypeInfo.cpp
    Source/Unhandled/UI/FontAssetTypeInfo.h
    Source/Unhandled/UI/UICanvasAssetTypeInfo.cpp
    Source/Unhandled/UI/UICanvasAssetTypeInfo.h
)

set(SKIP_UNITY_BUILD_INCLUSION_FILES
    Source/Shape/ShapeComponent.cpp
)
