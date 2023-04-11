#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Include/Config/SceneProcessingConfigBus.h
    Source/Config/Components/SceneProcessingConfigSystemComponent.h
    Source/Config/Components/SceneProcessingConfigSystemComponent.cpp
    Source/Config/Components/SoftNameBehavior.h
    Source/Config/Components/SoftNameBehavior.cpp
    Source/Generation/Components/TangentGenerator/TangentGenerateComponent.h
    Source/Generation/Components/TangentGenerator/TangentGenerateComponent.cpp
    Source/Generation/Components/TangentGenerator/TangentPreExportComponent.h
    Source/Generation/Components/TangentGenerator/TangentPreExportComponent.cpp
    Source/Generation/Components/TangentGenerator/TangentGenerators/MikkTGenerator.h
    Source/Generation/Components/TangentGenerator/TangentGenerators/MikkTGenerator.cpp
    Source/Generation/Components/TangentGenerator/TangentGenerators/BlendShapeMikkTGenerator.h
    Source/Generation/Components/TangentGenerator/TangentGenerators/BlendShapeMikkTGenerator.cpp
    Source/Generation/Components/UVsGenerator/UVsGenerateComponent.h
    Source/Generation/Components/UVsGenerator/UVsGenerateComponent.cpp
    Source/Generation/Components/UVsGenerator/UVsPreExportComponent.h
    Source/Generation/Components/UVsGenerator/UVsPreExportComponent.cpp
    Source/Generation/Components/UVsGenerator/UVsGenerators/SphereMappingUVsGenerator.h
    Source/Generation/Components/UVsGenerator/UVsGenerators/SphereMappingUVsGenerator.cpp
    Source/Generation/Components/MeshOptimizer/MeshBuilder.cpp
    Source/Generation/Components/MeshOptimizer/MeshBuilder.h
    Source/Generation/Components/MeshOptimizer/MeshBuilderInvalidIndex.h
    Source/Generation/Components/MeshOptimizer/MeshBuilderSkinningInfo.cpp
    Source/Generation/Components/MeshOptimizer/MeshBuilderSkinningInfo.h
    Source/Generation/Components/MeshOptimizer/MeshBuilderSubMesh.cpp
    Source/Generation/Components/MeshOptimizer/MeshBuilderSubMesh.h
    Source/Generation/Components/MeshOptimizer/MeshBuilderVertexAttributeLayers.cpp
    Source/Generation/Components/MeshOptimizer/MeshBuilderVertexAttributeLayers.h
    Source/Generation/Components/MeshOptimizer/MeshOptimizerComponent.cpp
    Source/Generation/Components/MeshOptimizer/MeshOptimizerComponent.h
    Source/Config/SettingsObjects/SoftNameSetting.h
    Source/Config/SettingsObjects/SoftNameSetting.cpp
    Source/Config/SettingsObjects/NodeSoftNameSetting.h
    Source/Config/SettingsObjects/NodeSoftNameSetting.cpp
    Source/Config/SettingsObjects/FileSoftNameSetting.h
    Source/Config/SettingsObjects/FileSoftNameSetting.cpp
    Source/Config/Widgets/GraphTypeSelector.h
    Source/Config/Widgets/GraphTypeSelector.cpp
    Source/SceneBuilder/SceneBuilderComponent.h
    Source/SceneBuilder/SceneBuilderComponent.cpp
    Source/SceneBuilder/SceneBuilderWorker.h
    Source/SceneBuilder/SceneBuilderWorker.cpp
    Source/SceneBuilder/SceneSerializationHandler.h
    Source/SceneBuilder/SceneSerializationHandler.cpp
    Source/SceneBuilder/TraceMessageHook.h
    Source/SceneBuilder/TraceMessageHook.cpp
)
