#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    3rdParty/ACES/ACES/Aces.h
    Include/Atom/Feature/RenderCommon.h
    Include/Atom/Feature/ACES/AcesDisplayMapperFeatureProcessor.h
    Include/Atom/Feature/Automation/AtomAutomationBus.h
    Include/Atom/Feature/AuxGeom/AuxGeomFeatureProcessor.h
    Include/Atom/Feature/ColorGrading/LutResolution.h
    Include/Atom/Feature/CoreLights/CoreLightsConstants.h
    Include/Atom/Feature/DisplayMapper/AcesOutputTransformPass.h
    Include/Atom/Feature/DisplayMapper/AcesOutputTransformLutPass.h
    Include/Atom/Feature/DisplayMapper/ApplyShaperLookupTablePass.h
    Include/Atom/Feature/DisplayMapper/DisplayMapperPass.h
    Include/Atom/Feature/DisplayMapper/DisplayMapperFullScreenPass.h
    Include/Atom/Feature/DisplayMapper/BakeAcesOutputTransformLutPass.h
    Include/Atom/Feature/DisplayMapper/DisplayMapperConfigurationDescriptor.h
    Include/Atom/Feature/DisplayMapper/OutputTransformPass.h
    Include/Atom/Feature/ImGui/ImGuiUtils.h
    Include/Atom/Feature/ImGui/SystemBus.h
    Include/Atom/Feature/ImageBasedLights/ImageBasedLightFeatureProcessor.h
    Include/Atom/Feature/LookupTable/LookupTableAsset.h
    Include/Atom/Feature/Mesh/MeshFeatureProcessor.h
    Include/Atom/Feature/Mesh/ModelReloaderSystemInterface.h
    Include/Atom/Feature/PostProcessing/PostProcessingConstants.h
    Include/Atom/Feature/PostProcessing/SMAAFeatureProcessorInterface.h
    Include/Atom/Feature/PostProcess/PostFxLayerCategoriesConstants.h
    Include/Atom/Feature/ReflectionProbe/ReflectionProbeFeatureProcessorInterface.h
    Include/Atom/Feature/SkyBox/SkyBoxFogBus.h
    Include/Atom/Feature/SkyBox/SkyboxConstants.h
    Include/Atom/Feature/SkyBox/SkyBoxLUT.h
    Include/Atom/Feature/SphericalHarmonics/SphericalHarmonicsUtility.h
    Include/Atom/Feature/SphericalHarmonics/SphericalHarmonicsUtility.inl
    Include/Atom/Feature/TransformService/TransformServiceFeatureProcessor.h
    Include/Atom/Feature/Utils/FrameCaptureBus.h
    Include/Atom/Feature/Utils/GpuBufferHandler.h
    Include/Atom/Feature/Utils/IndexedDataVector.h
    Include/Atom/Feature/Utils/IndexedDataVector.inl
    Include/Atom/Feature/Utils/MultiIndexedDataVector.h
    Include/Atom/Feature/Utils/MultiSparseVector.h
    Include/Atom/Feature/Utils/ProfilingCaptureBus.h
    Include/Atom/Feature/Utils/SparseVector.h
    Source/CommonModule.cpp
    Source/CommonSystemComponent.cpp
    Source/FrameCaptureSystemComponent.cpp
    Source/FrameCaptureSystemComponent.h
    Source/ProfilingCaptureSystemComponent.cpp
    Source/ProfilingCaptureSystemComponent.h
    3rdParty/ACES/ACES/Aces.cpp
    Source/ACES/AcesDisplayMapperFeatureProcessor.cpp
    Source/AuxGeom/AuxGeomBase.h
    Source/AuxGeom/AuxGeomFeatureProcessor.cpp
    Source/AuxGeom/AuxGeomDrawQueue.cpp
    Source/AuxGeom/AuxGeomDrawQueue.h
    Source/AuxGeom/AuxGeomDrawProcessorShared.cpp
    Source/AuxGeom/AuxGeomDrawProcessorShared.h
    Source/AuxGeom/DynamicPrimitiveProcessor.cpp
    Source/AuxGeom/DynamicPrimitiveProcessor.h
    Source/AuxGeom/FixedShapeProcessor.cpp
    Source/AuxGeom/FixedShapeProcessor.h
    Source/ColorGrading/LutGenerationPass.cpp
    Source/ColorGrading/LutGenerationPass.h
    Source/CoreLights/CapsuleLightFeatureProcessor.h
    Source/CoreLights/CapsuleLightFeatureProcessor.cpp
    Source/CoreLights/CascadedShadowmapsPass.h
    Source/CoreLights/CascadedShadowmapsPass.cpp
    Source/CoreLights/CoreLightsSystemComponent.h
    Source/CoreLights/CoreLightsSystemComponent.cpp
    Source/CoreLights/DepthExponentiationPass.h
    Source/CoreLights/DepthExponentiationPass.cpp
    Source/CoreLights/DirectionalLightFeatureProcessor.h
    Source/CoreLights/DirectionalLightFeatureProcessor.cpp
    Source/CoreLights/DiskLightFeatureProcessor.h
    Source/CoreLights/DiskLightFeatureProcessor.cpp
    Source/CoreLights/EsmShadowmapsPass.h
    Source/CoreLights/EsmShadowmapsPass.cpp
    Source/CoreLights/LtcCommon.h
    Source/CoreLights/LtcCommon.cpp
    Source/CoreLights/PointLightFeatureProcessor.h
    Source/CoreLights/PointLightFeatureProcessor.cpp
    Source/CoreLights/PolygonLightFeatureProcessor.h
    Source/CoreLights/PolygonLightFeatureProcessor.cpp
    Source/CoreLights/ProjectedShadowmapsPass.h
    Source/CoreLights/ProjectedShadowmapsPass.cpp
    Source/CoreLights/QuadLightFeatureProcessor.h
    Source/CoreLights/QuadLightFeatureProcessor.cpp
    Source/CoreLights/SimplePointLightFeatureProcessor.h
    Source/CoreLights/SimplePointLightFeatureProcessor.cpp
    Source/CoreLights/SimpleSpotLightFeatureProcessor.h
    Source/CoreLights/SimpleSpotLightFeatureProcessor.cpp
    Source/CoreLights/Shadow.h
    Source/CoreLights/Shadow.cpp
    Source/CoreLights/ShadowmapAtlas.h
    Source/CoreLights/ShadowmapAtlas.cpp
    Source/CoreLights/ShadowmapPass.h
    Source/CoreLights/ShadowmapPass.cpp
    Source/CoreLights/LightCullingPass.cpp
    Source/CoreLights/LightCullingPass.h
    Source/CoreLights/LightCullingTilePreparePass.cpp
    Source/CoreLights/LightCullingTilePreparePass.h
    Source/CoreLights/LightCullingRemap.cpp
    Source/CoreLights/LightCullingRemap.h
    Source/CoreLights/LightCullingConstants.h
    Source/Checkerboard/CheckerboardColorResolvePass.cpp
    Source/Checkerboard/CheckerboardColorResolvePass.h
    Source/Checkerboard/CheckerboardPass.cpp
    Source/Checkerboard/CheckerboardPass.h
    Source/Decals/DecalFeatureProcessor.h
    Source/Decals/DecalFeatureProcessor.cpp
    Source/Decals/DecalTextureArray.h
    Source/Decals/DecalTextureArray.cpp
    Source/Decals/AsyncLoadTracker.h
    Source/Decals/DecalTextureArrayFeatureProcessor.h
    Source/Decals/DecalTextureArrayFeatureProcessor.cpp    
    Source/DiffuseGlobalIllumination/DiffuseProbeGridRayTracingPass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridRayTracingPass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridBlendIrradiancePass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridBlendIrradiancePass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridBlendDistancePass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridBlendDistancePass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridBorderUpdatePass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridBorderUpdatePass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridRelocationPass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridRelocationPass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridClassificationPass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridClassificationPass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridDownsamplePass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridDownsamplePass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridRenderPass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridRenderPass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGrid.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGrid.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridTextureReadback.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridTextureReadback.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridFeatureProcessor.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridFeatureProcessor.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridVisualizationPreparePass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridVisualizationPreparePass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridVisualizationAccelerationStructurePass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridVisualizationAccelerationStructurePass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridVisualizationRayTracingPass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridVisualizationRayTracingPass.cpp
    Source/DiffuseGlobalIllumination/DiffuseProbeGridVisualizationCompositePass.h
    Source/DiffuseGlobalIllumination/DiffuseProbeGridVisualizationCompositePass.cpp
    Source/DiffuseGlobalIllumination/DiffuseGlobalIlluminationFeatureProcessor.h
    Source/DiffuseGlobalIllumination/DiffuseGlobalIlluminationFeatureProcessor.cpp
    Source/DisplayMapper/AcesOutputTransformPass.cpp
    Source/DisplayMapper/AcesOutputTransformLutPass.cpp
    Source/DisplayMapper/ApplyShaperLookupTablePass.cpp
    Source/DisplayMapper/DisplayMapperPass.cpp
    Source/DisplayMapper/DisplayMapperFullScreenPass.cpp
    Source/DisplayMapper/BakeAcesOutputTransformLutPass.cpp
    Source/DisplayMapper/DisplayMapperConfigurationDescriptor.cpp
    Source/DisplayMapper/OutputTransformPass.cpp
    Source/ImGui/ImGuiPass.cpp
    Source/ImGui/ImGuiPass.h
    Source/ImGui/ImGuiSystemComponent.cpp
    Source/ImGui/ImGuiSystemComponent.h
    Source/ImageBasedLights/ImageBasedLightFeatureProcessor.cpp
    Source/LookupTable/LookupTableAsset.cpp
    Source/Material/ConvertEmissiveUnitFunctor.cpp
    Source/Material/ConvertEmissiveUnitFunctor.h
    Source/Material/SubsurfaceTransmissionParameterFunctor.cpp
    Source/Material/SubsurfaceTransmissionParameterFunctor.h
    Source/Material/Transform2DFunctor.cpp
    Source/Material/Transform2DFunctor.h
    Source/Material/UseTextureFunctor.cpp
    Source/Material/UseTextureFunctor.h
    Source/Material/DrawListFunctor.cpp
    Source/Material/DrawListFunctor.h
    Source/Math/GaussianMathFilter.h
    Source/Math/GaussianMathFilter.cpp
    Source/Math/MathFilter.h
    Source/Math/MathFilter.cpp
    Source/Math/MathFilterDescriptor.h
    Source/Mesh/MeshFeatureProcessor.cpp
    Source/Mesh/ModelReloader.cpp
    Source/Mesh/ModelReloader.h
    Source/Mesh/ModelReloaderSystem.cpp
    Source/Mesh/ModelReloaderSystem.h
    Source/MorphTargets/MorphTargetComputePass.cpp
    Source/MorphTargets/MorphTargetComputePass.h
    Source/MorphTargets/MorphTargetDispatchItem.cpp
    Source/MorphTargets/MorphTargetDispatchItem.h
    Source/OcclusionCullingPlane/OcclusionCullingPlaneFeatureProcessor.h
    Source/OcclusionCullingPlane/OcclusionCullingPlaneFeatureProcessor.cpp
    Source/OcclusionCullingPlane/OcclusionCullingPlane.h
    Source/OcclusionCullingPlane/OcclusionCullingPlane.cpp
    Source/PostProcess/PostProcessBase.cpp
    Source/PostProcess/PostProcessBase.h
    Source/PostProcess/PostProcessFeatureProcessor.cpp
    Source/PostProcess/PostProcessFeatureProcessor.h
    Source/PostProcess/PostProcessSettings.cpp
    Source/PostProcess/PostProcessSettings.h
    Source/PostProcess/ColorGrading/HDRColorGradingSettings.h
    Source/PostProcess/ColorGrading/HDRColorGradingSettings.cpp
    Source/PostProcess/EditorModeFeedback/EditorModeFeedbackSettings.h
    Source/PostProcess/EditorModeFeedback/EditorModeFeedbackSettings.cpp
    Source/PostProcess/Bloom/BloomSettings.cpp
    Source/PostProcess/Bloom/BloomSettings.h
    Source/PostProcess/DepthOfField/DepthOfFieldSettings.cpp
    Source/PostProcess/DepthOfField/DepthOfFieldSettings.h
    Source/PostProcess/ExposureControl/ExposureControlSettings.cpp
    Source/PostProcess/ExposureControl/ExposureControlSettings.h
    Source/PostProcess/Ssao/SsaoSettings.cpp
    Source/PostProcess/Ssao/SsaoSettings.h
    Source/PostProcess/LookModification/LookModificationSettings.cpp
    Source/PostProcess/LookModification/LookModificationSettings.h
    Source/PostProcessing/BlendColorGradingLutsPass.h
    Source/PostProcessing/BlendColorGradingLutsPass.cpp
    Source/PostProcessing/DepthOfFieldPencilMap.h
    Source/PostProcessing/DepthOfFieldPencilMap.h
    Source/PostProcessing/BloomDownsamplePass.h
    Source/PostProcessing/BloomDownsamplePass.cpp
    Source/PostProcessing/BloomBlurPass.h
    Source/PostProcessing/BloomBlurPass.cpp
    Source/PostProcessing/BloomCompositePass.h
    Source/PostProcessing/BloomCompositePass.cpp
    Source/PostProcessing/BloomParentPass.h
    Source/PostProcessing/BloomParentPass.cpp
    Source/PostProcessing/EditorModeFeedbackParentPass.cpp
    Source/PostProcessing/EditorModeFeedbackParentPass.h
    Source/PostProcessing/EditorModeFeedbackDepthTransition.cpp
    Source/PostProcessing/EditorModeFeedbackDepthTransition.h
    Source/PostProcessing/EditorModeMaskPass.cpp
    Source/PostProcessing/EditorModeMaskPass.h
    Source/PostProcessing/EditorModeDesaturationPass.cpp
    Source/PostProcessing/EditorModeDesaturationPass.h
    Source/PostProcessing/EditorModeTintPass.cpp
    Source/PostProcessing/EditorModeTintPass.h
    Source/PostProcessing/EditorModeBlurPass.cpp
    Source/PostProcessing/EditorModeBlurPass.h
    Source/PostProcessing/EditorModeOutlinePass.cpp
    Source/PostProcessing/EditorModeOutlinePass.h
    Source/PostProcessing/DepthOfFieldCompositePass.h
    Source/PostProcessing/DepthOfFieldCompositePass.cpp
    Source/PostProcessing/DepthOfFieldBokehBlurPass.h
    Source/PostProcessing/DepthOfFieldBokehBlurPass.cpp
    Source/PostProcessing/DepthOfFieldMaskPass.h
    Source/PostProcessing/DepthOfFieldMaskPass.cpp
    Source/PostProcessing/DepthOfFieldParentPass.h
    Source/PostProcessing/DepthOfFieldParentPass.cpp
    Source/PostProcessing/DepthOfFieldReadBackFocusDepthPass.h
    Source/PostProcessing/DepthOfFieldReadBackFocusDepthPass.cpp
    Source/PostProcessing/DepthOfFieldWriteFocusDepthFromGpuPass.h
    Source/PostProcessing/DepthOfFieldWriteFocusDepthFromGpuPass.cpp
    Source/PostProcessing/DepthOfFieldCopyFocusDepthToCpuPass.h
    Source/PostProcessing/DepthOfFieldCopyFocusDepthToCpuPass.cpp
    Source/PostProcessing/DepthUpsamplePass.cpp
    Source/PostProcessing/DepthUpsamplePass.h
    Source/PostProcessing/EyeAdaptationPass.cpp
    Source/PostProcessing/EyeAdaptationPass.h
    Source/PostProcessing/FastDepthAwareBlurPasses.cpp
    Source/PostProcessing/FastDepthAwareBlurPasses.h
    Source/PostProcessing/HDRColorGradingPass.cpp
    Source/PostProcessing/HDRColorGradingPass.h
    Source/PostProcessing/LookModificationCompositePass.cpp
    Source/PostProcessing/LookModificationCompositePass.h
    Source/PostProcessing/LookModificationTransformPass.cpp
    Source/PostProcessing/LookModificationTransformPass.h
    Source/PostProcessing/LuminanceHistogramGeneratorPass.h
    Source/PostProcessing/LuminanceHistogramGeneratorPass.cpp
    Source/PostProcessing/NewDepthOfFieldPasses.cpp
    Source/PostProcessing/NewDepthOfFieldPasses.h
    Source/PostProcessing/PostProcessingShaderOptionBase.cpp
    Source/PostProcessing/PostProcessingShaderOptionBase.h
    Source/PostProcessing/SMAABasePass.cpp
    Source/PostProcessing/SMAABasePass.h
    Source/PostProcessing/SMAABlendingWeightCalculationPass.cpp
    Source/PostProcessing/SMAABlendingWeightCalculationPass.h
    Source/PostProcessing/SMAACommon.h
    Source/PostProcessing/SMAAConfigurationDescriptor.cpp
    Source/PostProcessing/SMAAConfigurationDescriptor.h
    Source/PostProcessing/SMAAEdgeDetectionPass.cpp
    Source/PostProcessing/SMAAEdgeDetectionPass.h
    Source/PostProcessing/SMAAFeatureProcessor.cpp
    Source/PostProcessing/SMAAFeatureProcessor.h
    Source/PostProcessing/SMAANeighborhoodBlendingPass.cpp
    Source/PostProcessing/SMAANeighborhoodBlendingPass.h
    Source/PostProcessing/SsaoPasses.cpp
    Source/PostProcessing/SsaoPasses.h
    Source/PostProcessing/SubsurfaceScatteringPass.cpp
    Source/PostProcessing/SubsurfaceScatteringPass.h
    Source/PostProcessing/TaaPass.h
    Source/PostProcessing/TaaPass.cpp
    Source/RayTracing/RayTracingFeatureProcessor.h
    Source/RayTracing/RayTracingFeatureProcessor.cpp
    Source/RayTracing/RayTracingAccelerationStructurePass.cpp
    Source/RayTracing/RayTracingAccelerationStructurePass.h
    Source/RayTracing/RayTracingPass.cpp
    Source/RayTracing/RayTracingPass.h
    Source/RayTracing/RayTracingPassData.h
    Source/ReflectionProbe/ReflectionProbeFeatureProcessor.h
    Source/ReflectionProbe/ReflectionProbeFeatureProcessor.cpp
    Source/ReflectionProbe/ReflectionProbe.h
    Source/ReflectionProbe/ReflectionProbe.cpp
    Source/ReflectionScreenSpace/ReflectionScreenSpaceTracePass.cpp
    Source/ReflectionScreenSpace/ReflectionScreenSpaceTracePass.h
    Source/ReflectionScreenSpace/ReflectionScreenSpaceBlurPass.cpp
    Source/ReflectionScreenSpace/ReflectionScreenSpaceBlurPass.h
    Source/ReflectionScreenSpace/ReflectionScreenSpaceBlurChildPass.cpp
    Source/ReflectionScreenSpace/ReflectionScreenSpaceBlurChildPass.h
    Source/ReflectionScreenSpace/ReflectionScreenSpaceCompositePass.cpp
    Source/ReflectionScreenSpace/ReflectionScreenSpaceCompositePass.h
    Source/ReflectionScreenSpace/ReflectionCopyFrameBufferPass.cpp
    Source/ReflectionScreenSpace/ReflectionCopyFrameBufferPass.h
    Source/ScreenSpace/DeferredFogSettings.cpp
    Source/ScreenSpace/DeferredFogSettings.h
    Source/ScreenSpace/DeferredFogPass.cpp
    Source/ScreenSpace/DeferredFogPass.h
    Source/Shadows/ProjectedShadowFeatureProcessor.h
    Source/Shadows/ProjectedShadowFeatureProcessor.cpp
    Source/SkinnedMesh/SkinnedMeshComputePass.cpp
    Source/SkinnedMesh/SkinnedMeshComputePass.h
    Source/SkinnedMesh/SkinnedMeshDispatchItem.cpp
    Source/SkinnedMesh/SkinnedMeshDispatchItem.h
    Source/SkinnedMesh/SkinnedMeshFeatureProcessor.cpp
    Source/SkinnedMesh/SkinnedMeshFeatureProcessor.h
    Source/SkinnedMesh/SkinnedMeshOutputStreamManager.cpp
    Source/SkinnedMesh/SkinnedMeshOutputStreamManager.h
    Source/SkinnedMesh/SkinnedMeshRenderProxy.cpp
    Source/SkinnedMesh/SkinnedMeshRenderProxy.h
    Source/SkinnedMesh/SkinnedMeshShaderOptionsCache.cpp
    Source/SkinnedMesh/SkinnedMeshShaderOptionsCache.h
    Source/SkinnedMesh/SkinnedMeshStatsCollector.cpp
    Source/SkinnedMesh/SkinnedMeshStatsCollector.h
    Source/SkinnedMesh/SkinnedMeshSystemComponent.cpp
    Source/SkinnedMesh/SkinnedMeshSystemComponent.h
    Source/SkinnedMesh/SkinnedMeshVertexStreamProperties.cpp
    Source/SkinnedMesh/SkinnedMeshVertexStreamProperties.h
    Source/SkyBox/SkyBoxFeatureProcessor.cpp
    Source/SkyBox/SkyBoxFeatureProcessor.h
    Source/SkyBox/SkyBoxFogSettings.h
    Source/SkyBox/SkyBoxFogSettings.cpp
    Source/TransformService/TransformServiceFeatureProcessor.cpp
    Source/Utils/GpuBufferHandler.cpp
)

set(SKIP_UNITY_BUILD_INCLUSION_FILES
    Source/CommonModule.cpp
)
