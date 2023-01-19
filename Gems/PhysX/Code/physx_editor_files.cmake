#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Include/PhysX/EditorColliderComponentRequestBus.h
    Include/PhysX/EditorHeightfieldColliderRequestBus.h
    Include/PhysX/EditorJointBus.h
    Source/ComponentDescriptors.cpp
    Source/ComponentDescriptors.h
    Source/EditorComponentDescriptors.cpp
    Source/EditorComponentDescriptors.h
    Source/EditorRigidBodyComponent.cpp
    Source/EditorRigidBodyComponent.h
    Source/EditorColliderComponent.cpp
    Source/EditorColliderComponent.h
    Source/EditorShapeColliderComponent.cpp
    Source/EditorShapeColliderComponent.h
    Source/EditorForceRegionComponent.cpp
    Source/EditorForceRegionComponent.h
    Source/EditorBallJointComponent.cpp
    Source/EditorBallJointComponent.h
    Source/EditorFixedJointComponent.cpp
    Source/EditorFixedJointComponent.h
    Source/EditorHingeJointComponent.cpp
    Source/EditorHingeJointComponent.h
    Source/EditorPrismaticJointComponent.cpp
    Source/EditorPrismaticJointComponent.h
    Source/EditorHeightfieldColliderComponent.cpp
    Source/EditorHeightfieldColliderComponent.h
    Source/EditorJointComponent.cpp
    Source/EditorJointComponent.h
    Source/EditorPhysXJointInterface.cpp
    Source/EditorPhysXJointInterface.h
    Source/EditorJointOptimizer.cpp
    Source/EditorJointOptimizer.h
    Source/Pipeline/MeshExporter.cpp
    Source/Pipeline/MeshExporter.h
    Source/Pipeline/MeshGroup.cpp
    Source/Pipeline/MeshGroup.h
    Source/Pipeline/MeshBehavior.cpp
    Source/Pipeline/MeshBehavior.h
    Source/Pipeline/PrimitiveShapeFitter/PrimitiveShapeFitter.cpp
    Source/Pipeline/PrimitiveShapeFitter/PrimitiveShapeFitter.h
    Source/Pipeline/PrimitiveShapeFitter/AbstractShapeParameterization.cpp
    Source/Pipeline/PrimitiveShapeFitter/AbstractShapeParameterization.h
    Source/Pipeline/PrimitiveShapeFitter/Utils.cpp
    Source/Pipeline/PrimitiveShapeFitter/Utils.h
    Source/PhysXCharacters/Components/EditorCharacterControllerComponent.cpp
    Source/PhysXCharacters/Components/EditorCharacterControllerComponent.h
    Source/PhysXCharacters/Components/EditorCharacterGameplayComponent.cpp
    Source/PhysXCharacters/Components/EditorCharacterGameplayComponent.h
    Editor/ConfigurationWidget.h
    Editor/ConfigurationWidget.cpp
    Editor/EditorWindow.h
    Editor/EditorWindow.cpp
    Editor/EditorWindow.ui
    Editor/SettingsWidget.h
    Editor/SettingsWidget.cpp
    Editor/CollisionLayersWidget.h
    Editor/CollisionLayersWidget.cpp
    Editor/CollisionLayerWidget.h
    Editor/CollisionLayerWidget.cpp
    Editor/CollisionGroupWidget.h
    Editor/CollisionGroupWidget.cpp
    Editor/CollisionGroupsWidget.h
    Editor/CollisionGroupsWidget.cpp
    Editor/CollisionFilteringWidget.h
    Editor/CollisionFilteringWidget.cpp
    Editor/PropertyTypes.h
    Editor/PropertyTypes.cpp
    Editor/EditorClassConverters.cpp
    Editor/EditorClassConverters.h
    Editor/PvdWidget.h
    Editor/PvdWidget.cpp
    Editor/DocumentationLinkWidget.h
    Editor/DocumentationLinkWidget.cpp
    Editor/InertiaPropertyHandler.h
    Editor/InertiaPropertyHandler.cpp
    Editor/ComboBoxEditButtonPair.h
    Editor/ComboBoxEditButtonPair.cpp
    Editor/ColliderComponentMode.h
    Editor/ColliderComponentMode.cpp
    Editor/ColliderOffsetMode.h
    Editor/ColliderOffsetMode.cpp
    Editor/ColliderBoxMode.h
    Editor/ColliderBoxMode.cpp
    Editor/ColliderRotationMode.h
    Editor/ColliderRotationMode.cpp
    Editor/ColliderSphereMode.h
    Editor/ColliderSphereMode.cpp
    Editor/ColliderCapsuleMode.h
    Editor/ColliderCapsuleMode.cpp
    Editor/ColliderAssetScaleMode.h
    Editor/ColliderAssetScaleMode.cpp
    Editor/DebugDraw.cpp
    Editor/DebugDraw.h
    Editor/PolygonPrismMeshUtils.cpp
    Editor/PolygonPrismMeshUtils.h
    Editor/EditorJointCommon.h
    Editor/EditorJointConfiguration.cpp
    Editor/EditorJointConfiguration.h
    Editor/EditorViewportEntityPicker.cpp
    Editor/EditorViewportEntityPicker.h

    Editor/Source/Components/EditorSystemComponent.h
    Editor/Source/Components/EditorSystemComponent.cpp
    Editor/Source/ComponentModes/Joints/JointsComponentMode.h
    Editor/Source/ComponentModes/Joints/JointsComponentMode.cpp
    Editor/Source/ComponentModes/Joints/JointsComponentModeBus.h
    Editor/Source/ComponentModes/Joints/JointsComponentModeCommon.h
    Editor/Source/ComponentModes/Joints/JointsComponentModeCommon.cpp
    Editor/Source/ComponentModes/PhysXSubComponentModeBase.h
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeAngleCone.h
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeAngleCone.cpp
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeAnglePair.h
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeAnglePair.cpp
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeLinearFloat.h
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeLinearFloat.cpp
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeRotation.h
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeRotation.cpp
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeSnap.h
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeSnap.cpp
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeSnapPosition.h
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeSnapPosition.cpp
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeSnapRotation.h
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeSnapRotation.cpp
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeTranslate.h
    Editor/Source/ComponentModes/Joints/JointsSubComponentModeTranslate.cpp
    Editor/Source/Configuration/PhysXEditorSettingsRegistryManager.h
    Editor/Source/Configuration/PhysXEditorSettingsRegistryManager.cpp
    Editor/Source/Material/PhysXEditorMaterialAsset.cpp
    Editor/Source/Material/PhysXEditorMaterialAsset.h
    Editor/Source/Material/PhysXEditorMaterialAssetBuilder.cpp
    Editor/Source/Material/PhysXEditorMaterialAssetBuilder.h
)
