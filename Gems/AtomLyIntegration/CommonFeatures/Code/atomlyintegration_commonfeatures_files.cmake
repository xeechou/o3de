#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Source/Animation/AttachmentComponent.h
    Source/Animation/AttachmentComponent.cpp
    Source/CoreLights/AreaLightComponent.h
    Source/CoreLights/AreaLightComponent.cpp
    Source/CoreLights/AreaLightComponentConfig.cpp
    Source/CoreLights/AreaLightComponentController.h
    Source/CoreLights/AreaLightComponentController.cpp
    Source/CoreLights/CapsuleLightDelegate.h
    Source/CoreLights/CapsuleLightDelegate.cpp
    Source/CoreLights/DirectionalLightComponent.h
    Source/CoreLights/DirectionalLightComponent.cpp
    Source/CoreLights/DirectionalLightComponentController.h
    Source/CoreLights/DirectionalLightComponentController.cpp
    Source/CoreLights/DirectionalLightComponentConfig.cpp
    Source/CoreLights/DiskLightDelegate.h
    Source/CoreLights/DiskLightDelegate.cpp
    Source/CoreLights/LightDelegateBase.h
    Source/CoreLights/LightDelegateBase.inl
    Source/CoreLights/LightDelegateInterface.h
    Source/CoreLights/PolygonLightDelegate.h
    Source/CoreLights/PolygonLightDelegate.cpp
    Source/CoreLights/QuadLightDelegate.h
    Source/CoreLights/QuadLightDelegate.cpp
    Source/CoreLights/SimplePointLightDelegate.h
    Source/CoreLights/SimplePointLightDelegate.cpp
    Source/CoreLights/SimpleSpotLightDelegate.h
    Source/CoreLights/SimpleSpotLightDelegate.cpp
    Source/CoreLights/SphereLightDelegate.h
    Source/CoreLights/SphereLightDelegate.cpp
    Source/Decals/DecalComponent.h
    Source/Decals/DecalComponent.cpp
    Source/Decals/DecalComponentController.h
    Source/Decals/DecalComponentController.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridComponent.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridComponent.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridComponentController.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridComponentController.cpp
    Source/DiffuseGlobalIllumination/DiffuseGlobalIlluminationComponent.h
    Source/DiffuseGlobalIllumination/DiffuseGlobalIlluminationComponent.cpp
    Source/DiffuseGlobalIllumination/DiffuseGlobalIlluminationComponentController.h
    Source/DiffuseGlobalIllumination/DiffuseGlobalIlluminationComponentController.cpp
    Source/DiffuseGlobalIllumination/DiffuseGlobalIlluminationComponentConfig.h
    Source/DiffuseGlobalIllumination/DiffuseGlobalIlluminationComponentConfig.cpp
    Source/Grid/GridComponent.h
    Source/Grid/GridComponent.cpp
    Source/Grid/GridComponentConfig.cpp
    Source/Grid/GridComponentController.h
    Source/Grid/GridComponentController.cpp
    Source/ImageBasedLights/ImageBasedLightComponent.cpp
    Source/ImageBasedLights/ImageBasedLightComponent.h
    Source/ImageBasedLights/ImageBasedLightComponentConfig.cpp
    Source/ImageBasedLights/ImageBasedLightComponentController.cpp
    Source/ImageBasedLights/ImageBasedLightComponentController.h
    Source/Material/MaterialComponent.cpp
    Source/Material/MaterialComponent.h
    Source/Material/MaterialComponentConfig.cpp
    Source/Material/MaterialComponentController.cpp
    Source/Material/MaterialComponentController.h
    Source/Mesh/MeshComponent.h
    Source/Mesh/MeshComponent.cpp
    Source/Mesh/MeshComponentController.h
    Source/Mesh/MeshComponentController.cpp
    Source/OcclusionCullingPlane/OcclusionCullingPlaneComponent.h
    Source/OcclusionCullingPlane/OcclusionCullingPlaneComponent.cpp
    Source/OcclusionCullingPlane/OcclusionCullingPlaneComponentController.h
    Source/OcclusionCullingPlane/OcclusionCullingPlaneComponentController.cpp
    Source/PostProcess/PostFxLayerComponent.cpp
    Source/PostProcess/PostFxLayerComponent.h
    Source/PostProcess/PostFxLayerComponentConfig.cpp
    Source/PostProcess/PostFxLayerComponentController.cpp
    Source/PostProcess/PostFxLayerComponentController.h
    Source/PostProcess/Bloom/BloomComponent.cpp
    Source/PostProcess/Bloom/BloomComponent.h
    Source/PostProcess/Bloom/BloomComponentConfig.cpp
    Source/PostProcess/Bloom/BloomComponentController.cpp
    Source/PostProcess/Bloom/BloomComponentController.h
    Source/PostProcess/ColorGrading/HDRColorGradingComponent.cpp
    Source/PostProcess/ColorGrading/HDRColorGradingComponent.h
    Source/PostProcess/ColorGrading/HDRColorGradingComponentConfig.cpp
    Source/PostProcess/ColorGrading/HDRColorGradingComponentController.cpp
    Source/PostProcess/ColorGrading/HDRColorGradingComponentController.h
    Source/PostProcess/DepthOfField/DepthOfFieldComponent.cpp
    Source/PostProcess/DepthOfField/DepthOfFieldComponent.h
    Source/PostProcess/DepthOfField/DepthOfFieldComponentConfig.cpp
    Source/PostProcess/DepthOfField/DepthOfFieldComponentController.cpp
    Source/PostProcess/DepthOfField/DepthOfFieldComponentController.h
    Source/PostProcess/DisplayMapper/DisplayMapperComponent.cpp
    Source/PostProcess/DisplayMapper/DisplayMapperComponent.h
    Source/PostProcess/DisplayMapper/DisplayMapperComponentConfig.cpp
    Source/PostProcess/DisplayMapper/DisplayMapperComponentController.cpp
    Source/PostProcess/DisplayMapper/DisplayMapperComponentController.h
    Source/PostProcess/ExposureControl/ExposureControlComponent.cpp
    Source/PostProcess/ExposureControl/ExposureControlComponent.h
    Source/PostProcess/ExposureControl/ExposureControlComponentConfig.cpp
    Source/PostProcess/ExposureControl/ExposureControlComponentController.cpp
    Source/PostProcess/ExposureControl/ExposureControlComponentController.h
    Source/PostProcess/Ssao/SsaoComponent.cpp
    Source/PostProcess/Ssao/SsaoComponent.h
    Source/PostProcess/Ssao/SsaoComponentConfig.cpp
    Source/PostProcess/Ssao/SsaoComponentController.cpp
    Source/PostProcess/Ssao/SsaoComponentController.h
    Source/PostProcess/LookModification/LookModificationComponent.cpp
    Source/PostProcess/LookModification/LookModificationComponent.h
    Source/PostProcess/LookModification/LookModificationComponentConfig.cpp
    Source/PostProcess/LookModification/LookModificationComponentController.cpp
    Source/PostProcess/LookModification/LookModificationComponentController.h
    Source/PostProcess/RadiusWeightModifier/RadiusWeightModifierComponent.h
    Source/PostProcess/RadiusWeightModifier/RadiusWeightModifierComponent.cpp
    Source/PostProcess/RadiusWeightModifier/RadiusWeightModifierComponentConfig.cpp
    Source/PostProcess/RadiusWeightModifier/RadiusWeightModifierComponentController.h
    Source/PostProcess/RadiusWeightModifier/RadiusWeightModifierComponentController.cpp
    Source/PostProcess/ShapeWeightModifier/ShapeWeightModifierComponent.h
    Source/PostProcess/ShapeWeightModifier/ShapeWeightModifierComponent.cpp
    Source/PostProcess/ShapeWeightModifier/ShapeWeightModifierComponentConfig.cpp
    Source/PostProcess/ShapeWeightModifier/ShapeWeightModifierComponentController.h
    Source/PostProcess/ShapeWeightModifier/ShapeWeightModifierComponentController.cpp
    Source/PostProcess/GradientWeightModifier/GradientWeightModifierComponent.h
    Source/PostProcess/GradientWeightModifier/GradientWeightModifierComponent.cpp
    Source/PostProcess/GradientWeightModifier/GradientWeightModifierComponentConfig.cpp
    Source/PostProcess/GradientWeightModifier/GradientWeightModifierController.h
    Source/PostProcess/GradientWeightModifier/GradientWeightModifierController.cpp
    Source/ScreenSpace/DeferredFogComponentController.h
    Source/ScreenSpace/DeferredFogComponentController.cpp
    Source/ScreenSpace/DeferredFogComponent.h
    Source/ScreenSpace/DeferredFogComponent.cpp
    Source/ScreenSpace/DeferredFogComponentConfig.cpp
    Source/ReflectionProbe/ReflectionProbeComponent.h
    Source/ReflectionProbe/ReflectionProbeComponent.cpp
    Source/ReflectionProbe/ReflectionProbeComponentController.h
    Source/ReflectionProbe/ReflectionProbeComponentController.cpp
    Source/SkyBox/HDRiSkyboxComponentConfig.cpp
    Source/SkyBox/HDRiSkyboxComponentController.cpp
    Source/SkyBox/HDRiSkyboxComponentController.h
    Source/SkyBox/HDRiSkyboxComponent.cpp
    Source/SkyBox/HDRiSkyboxComponent.h
    Source/SkyBox/PhysicalSkyComponentConfig.cpp
    Source/SkyBox/PhysicalSkyComponentController.cpp
    Source/SkyBox/PhysicalSkyComponentController.h
    Source/SkyBox/PhysicalSkyComponent.cpp
    Source/SkyBox/PhysicalSkyComponent.h
    Source/CommonFeaturesSystemComponent.h
    Source/CommonFeaturesSystemComponent.cpp
    Source/Scripting/EntityReferenceComponent.cpp
    Source/Scripting/EntityReferenceComponent.h
    Source/Scripting/EntityReferenceComponentConfig.cpp
    Source/Scripting/EntityReferenceComponentController.cpp
    Source/Scripting/EntityReferenceComponentController.h
    Source/SurfaceData/SurfaceDataMeshComponent.cpp
    Source/SurfaceData/SurfaceDataMeshComponent.h
)
