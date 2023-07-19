#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Include/ScriptCanvas/SystemComponent.h
    Include/ScriptCanvas/ScriptCanvasGem.h
    Include/ScriptCanvas/Asset/AssetDescription.h
    Include/ScriptCanvas/Asset/AssetRegistry.h
    Include/ScriptCanvas/Asset/AssetRegistryBus.h
    Include/ScriptCanvas/Asset/ExecutionLogAsset.h
    Include/ScriptCanvas/Asset/ExecutionLogAssetBus.h
    Include/ScriptCanvas/Asset/RuntimeAsset.h
    Include/ScriptCanvas/Asset/RuntimeAssetHandler.h
    Include/ScriptCanvas/Asset/RuntimeInputs.h
    Include/ScriptCanvas/Asset/SubgraphInterfaceAsset.h
    Include/ScriptCanvas/Asset/SubgraphInterfaceAssetHandler.h
    Include/ScriptCanvas/Core/ScriptCanvasBus.h
    Include/ScriptCanvas/Core/ExecutionNotificationsBus.h
    Include/ScriptCanvas/Core/GraphBus.h
    Include/ScriptCanvas/Core/NodeBus.h
    Include/ScriptCanvas/Core/EBusNodeBus.h
    Include/ScriptCanvas/Core/NodelingBus.h
    Include/ScriptCanvas/Core/Attributes.h
    Include/ScriptCanvas/Core/Connection.h
    Include/ScriptCanvas/Core/ConnectionBus.h
    Include/ScriptCanvas/Core/Contract.h
    Include/ScriptCanvas/Core/Contracts.h
    Include/ScriptCanvas/Core/Contracts/MethodOverloadContract.h
    Include/ScriptCanvas/Core/Contracts/RestrictedNodeContract.h
    Include/ScriptCanvas/Core/Core.h
    Include/ScriptCanvas/Core/Datum.h
    Include/ScriptCanvas/Core/DatumBus.h
    Include/ScriptCanvas/Core/EBusHandler.h
    Include/ScriptCanvas/Core/Endpoint.h
    Include/ScriptCanvas/Core/Graph.h
    Include/ScriptCanvas/Core/GraphData.h
    Include/ScriptCanvas/Core/GraphSerialization.h
    Include/ScriptCanvas/Core/GraphScopedTypes.h
    Include/ScriptCanvas/Core/MethodConfiguration.h
    Include/ScriptCanvas/Core/ModifiableDatumView.h
    Include/ScriptCanvas/Core/NamedId.h
    Include/ScriptCanvas/Core/Node.h
    Include/ScriptCanvas/Core/Nodeable.h
    Include/ScriptCanvas/Core/NodeableNode.h
    Include/ScriptCanvas/Core/NodeableNodeOverloaded.h
    Include/ScriptCanvas/Core/NodeableOut.h
    Include/ScriptCanvas/Core/NodeFunctionGeneric.h
    Include/ScriptCanvas/Core/SerializationListener.h
    Include/ScriptCanvas/Core/Slot.h
    Include/ScriptCanvas/Core/SlotConfigurationDefaults.h
    Include/ScriptCanvas/Core/SlotConfigurations.h
    Include/ScriptCanvas/Core/SlotExecutionMap.h
    Include/ScriptCanvas/Core/SlotMetadata.h
    Include/ScriptCanvas/Core/SlotNames.h
    Include/ScriptCanvas/Core/SubgraphInterface.h
    Include/ScriptCanvas/Core/SubgraphInterfaceUtility.h
    Include/ScriptCanvas/CodeGen/NodeableCodegen.h
    Include/ScriptCanvas/Core/Contracts/ConnectionLimitContract.h
    Include/ScriptCanvas/Core/Contracts/ContractRTTI.h
    Include/ScriptCanvas/Core/Contracts/DisallowReentrantExecutionContract.h
    Include/ScriptCanvas/Core/Contracts/DisplayGroupConnectedSlotLimitContract.h
    Include/ScriptCanvas/Core/Contracts/DynamicTypeContract.h
    Include/ScriptCanvas/Core/Contracts/IsReferenceTypeContract.h
    Include/ScriptCanvas/Core/Contracts/MathOperatorContract.h
    Include/ScriptCanvas/Core/Contracts/SlotTypeContract.h
    Include/ScriptCanvas/Core/Contracts/SupportsMethodContract.h
    Include/ScriptCanvas/Core/Contracts/TypeContract.h
    Include/ScriptCanvas/Data/BehaviorContextObject.h
    Include/ScriptCanvas/Data/BehaviorContextObjectPtr.h
    Include/ScriptCanvas/Data/Constants.h
    Include/ScriptCanvas/Data/Data.h
    Include/ScriptCanvas/Data/DataRegistry.h
    Include/ScriptCanvas/Data/DataTrait.h
    Include/ScriptCanvas/Data/NumericData.h
    Include/ScriptCanvas/Data/PropertyTraits.h
    Include/ScriptCanvas/Data/Traits.h
    Include/ScriptCanvas/Deprecated/VariableDatumBase.h
    Include/ScriptCanvas/Deprecated/VariableDatum.h
    Include/ScriptCanvas/Deprecated/VariableHelpers.h
    Include/ScriptCanvas/Execution/ErrorBus.h
    Include/ScriptCanvas/Execution/Executor.h
    Include/ScriptCanvas/Execution/ExecutionBus.h
    Include/ScriptCanvas/Execution/ExecutionContext.h
    Include/ScriptCanvas/Execution/ExecutionObjectCloning.h
    Include/ScriptCanvas/Execution/ExecutionPerformanceTimer.h
    Include/ScriptCanvas/Execution/ExecutionState.h
    Include/ScriptCanvas/Execution/ExecutionStateDeclarations.h
    Include/ScriptCanvas/Execution/ExecutionStateHandler.h
    Include/ScriptCanvas/Execution/ExecutionStateStorage.h
    Include/ScriptCanvas/Execution/RuntimeComponent.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedAPI.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedClassAPI.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedCloningAPI.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedComponentAPI.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedDebugAPI.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedEBusAPI.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedOut.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpreted.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpretedAPI.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpretedPerActivation.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpretedPure.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpretedSingleton.h
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpretedUtility.h
    Include/ScriptCanvas/Grammar/AbstractCodeModel.h
    Include/ScriptCanvas/Grammar/ASTModifications.h
    Include/ScriptCanvas/Grammar/DebugMap.h
    Include/ScriptCanvas/Grammar/ExecutionTraversalListeners.h
    Include/ScriptCanvas/Grammar/ParsingMetaData.h
    Include/ScriptCanvas/Grammar/ParsingUtilities.h
    Include/ScriptCanvas/Grammar/ParsingUtilitiesScriptEventExtension.h
    Include/ScriptCanvas/Grammar/Primitives.h
    Include/ScriptCanvas/Grammar/PrimitivesDeclarations.h
    Include/ScriptCanvas/Grammar/PrimitivesExecution.h
    Include/ScriptCanvas/Grammar/SymbolNames.h
    Include/ScriptCanvas/Internal/Nodeables/BaseTimer.h
    Include/ScriptCanvas/Internal/Nodeables/BaseTimer.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Internal/Nodes/BaseTimerNode.h
    Include/ScriptCanvas/Internal/Nodes/BaseTimerNode.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Internal/Nodes/ExpressionNodeBase.h
    Include/ScriptCanvas/Internal/Nodes/ExpressionNodeBase.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Internal/Nodes/StringFormatted.h
    Include/ScriptCanvas/Internal/Nodes/StringFormatted.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Libraries.h
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactAddNodeable.h
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactAddNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactDecrementNodeable.h
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactDecrementNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactDivideNodeable.h
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactDivideNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactIncrementNodeable.h
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactIncrementNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactMultiplyNodeable.h
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactMultiplyNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactNegateNodeable.h
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactNegateNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactSubtractNodeable.h
    Include/ScriptCanvas/Libraries/Compact/BasicOperators/CompactSubtractNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactArccosineNodeable.h
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactArccosineNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactArcsineNodeable.h
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactArcsineNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactArctangent2Nodeable.h
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactArctangent2Nodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactArctangentNodeable.h
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactArctangentNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactCosineNodeable.h
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactCosineNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactSineNodeable.h
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactSineNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactTangentNodeable.h
    Include/ScriptCanvas/Libraries/Compact/Trigonometry/CompactTangentNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Core/AzEventHandler.h
    Include/ScriptCanvas/Libraries/Core/AzEventHandler.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/BinaryOperator.h
    Include/ScriptCanvas/Libraries/Core/CoreLibrary.h
    Include/ScriptCanvas/Libraries/Core/EBusEventHandler.h
    Include/ScriptCanvas/Libraries/Core/EBusEventHandler.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/ExtractProperty.h
    Include/ScriptCanvas/Libraries/Core/ExtractProperty.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/EventHandlerTranslationUtility.h
    Include/ScriptCanvas/Libraries/Core/ForEach.h
    Include/ScriptCanvas/Libraries/Core/ForEach.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/FunctionCallNode.h
    Include/ScriptCanvas/Libraries/Core/FunctionCallNode.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/FunctionCallNodeIsOutOfDate.h
    Include/ScriptCanvas/Libraries/Core/FunctionDefinitionNode.h
    Include/ScriptCanvas/Libraries/Core/FunctionDefinitionNode.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/GetVariable.h
    Include/ScriptCanvas/Libraries/Core/GetVariable.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/Method.h
    Include/ScriptCanvas/Libraries/Core/MethodOverloaded.h
    Include/ScriptCanvas/Libraries/Core/Nodeling.h
    Include/ScriptCanvas/Libraries/Core/Nodeling.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/ReceiveScriptEvent.h
    Include/ScriptCanvas/Libraries/Core/ReceiveScriptEvent.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/ScriptEventBase.h
    Include/ScriptCanvas/Libraries/Core/ScriptEventBase.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/SendScriptEvent.h
    Include/ScriptCanvas/Libraries/Core/SendScriptEvent.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/SetVariable.h
    Include/ScriptCanvas/Libraries/Core/SetVariable.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/Start.h
    Include/ScriptCanvas/Libraries/Core/Start.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Core/UnaryOperator.h
    Include/ScriptCanvas/Libraries/Deprecated/DeprecatedNodeLibrary.h
    Include/ScriptCanvas/Libraries/Entity/EntityFunctions.h
    Include/ScriptCanvas/Libraries/Entity/EntityFunctions.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Logic/And.h
    Include/ScriptCanvas/Libraries/Logic/Any.h
    Include/ScriptCanvas/Libraries/Logic/Any.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Logic/Break.h
    Include/ScriptCanvas/Libraries/Logic/Break.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Logic/Cycle.h
    Include/ScriptCanvas/Libraries/Logic/Cycle.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Logic/Gate.h
    Include/ScriptCanvas/Libraries/Logic/Gate.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Logic/IsNull.h
    Include/ScriptCanvas/Libraries/Logic/IsNull.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Logic/LogicLibrary.h
    Include/ScriptCanvas/Libraries/Logic/Not.h
    Include/ScriptCanvas/Libraries/Logic/Once.h
    Include/ScriptCanvas/Libraries/Logic/Once.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Logic/Or.h
    Include/ScriptCanvas/Libraries/Logic/OrderedSequencer.h
    Include/ScriptCanvas/Libraries/Logic/OrderedSequencer.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Logic/TargetedSequencer.h
    Include/ScriptCanvas/Libraries/Logic/TargetedSequencer.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Logic/WeightedRandomSequencer.h
    Include/ScriptCanvas/Libraries/Logic/WeightedRandomSequencer.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Logic/While.h
    Include/ScriptCanvas/Libraries/Logic/While.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Math/AABB.h
    Include/ScriptCanvas/Libraries/Math/AABB.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/Color.h
    Include/ScriptCanvas/Libraries/Math/Color.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/CRC.h
    Include/ScriptCanvas/Libraries/Math/CRC.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/MathExpression.h
    Include/ScriptCanvas/Libraries/Math/MathExpression.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Math/MathFunctions.h
    Include/ScriptCanvas/Libraries/Math/MathFunctions.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/MathNodeUtilities.h
    Include/ScriptCanvas/Libraries/Math/Matrix3x3.h
    Include/ScriptCanvas/Libraries/Math/Matrix3x3.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/Matrix4x4.h
    Include/ScriptCanvas/Libraries/Math/Matrix4x4.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/MatrixMxN.h
    Include/ScriptCanvas/Libraries/Math/MatrixMxN.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/OBB.h
    Include/ScriptCanvas/Libraries/Math/OBB.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/Plane.h
    Include/ScriptCanvas/Libraries/Math/Plane.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/Quaternion.h
    Include/ScriptCanvas/Libraries/Math/Quaternion.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/Transform.h
    Include/ScriptCanvas/Libraries/Math/Transform.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/Vector2.h
    Include/ScriptCanvas/Libraries/Math/Vector2.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/Vector3.h
    Include/ScriptCanvas/Libraries/Math/Vector3.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/Vector4.h
    Include/ScriptCanvas/Libraries/Math/Vector4.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Math/VectorN.h
    Include/ScriptCanvas/Libraries/Math/VectorN.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/Comparison/ComparisonLibrary.h
    Include/ScriptCanvas/Libraries/Comparison/EqualTo.h
    Include/ScriptCanvas/Libraries/Comparison/NotEqualTo.h
    Include/ScriptCanvas/Libraries/Comparison/Less.h
    Include/ScriptCanvas/Libraries/Comparison/Greater.h
    Include/ScriptCanvas/Libraries/Comparison/LessEqual.h
    Include/ScriptCanvas/Libraries/Comparison/GreaterEqual.h
    Include/ScriptCanvas/Libraries/Time/DelayNodeable.h
    Include/ScriptCanvas/Libraries/Time/DelayNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Time/TimeDelayNodeable.h
    Include/ScriptCanvas/Libraries/Time/TimeDelayNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Time/DurationNodeable.h
    Include/ScriptCanvas/Libraries/Time/DurationNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Time/HeartBeatNodeable.h
    Include/ScriptCanvas/Libraries/Time/HeartBeatNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Time/RepeaterNodeable.h
    Include/ScriptCanvas/Libraries/Time/RepeaterNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Time/TimerNodeable.h
    Include/ScriptCanvas/Libraries/Time/TimerNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Spawning/CreateSpawnTicketNodeable.h
    Include/ScriptCanvas/Libraries/Spawning/CreateSpawnTicketNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Spawning/SpawningData.ScriptCanvasData.xml
    Include/ScriptCanvas/Libraries/Spawning/SpawnNodeable.h
    Include/ScriptCanvas/Libraries/Spawning/SpawnNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/Spawning/DespawnNodeable.h
    Include/ScriptCanvas/Libraries/Spawning/DespawnNodeable.ScriptCanvasNodeable.xml
    Include/ScriptCanvas/Libraries/String/Format.h
    Include/ScriptCanvas/Libraries/String/Format.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/String/Print.h
    Include/ScriptCanvas/Libraries/String/Print.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/String/StringFunctions.h
    Include/ScriptCanvas/Libraries/String/StringFunctions.ScriptCanvasFunction.xml
    Include/ScriptCanvas/Libraries/UnitTesting/UnitTestingLibrary.h
    Include/ScriptCanvas/Libraries/UnitTesting/UnitTestBus.h
    Include/ScriptCanvas/Libraries/UnitTesting/UnitTestBusMacros.h
    Include/ScriptCanvas/Libraries/UnitTesting/Auxiliary/Auxiliary.h
    Include/ScriptCanvas/Libraries/UnitTesting/UnitTestBusSender.h
    Include/ScriptCanvas/Libraries/Operators/OperatorsLibrary.h
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorAdd.h
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorAdd.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorArithmetic.h
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorArithmetic.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorDiv.h
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorDiv.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorMul.h
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorMul.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorSub.h
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorSub.ScriptCanvasGrammar.xml
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorLerpNodeable.h
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorLerpNodeableNode.h
    Include/ScriptCanvas/PerformanceStatistician.h
    Include/ScriptCanvas/PerformanceStatisticsBus.h
    Include/ScriptCanvas/PerformanceTracker.h
    Include/ScriptCanvas/Results/ErrorText.h
    Include/ScriptCanvas/Serialization/BehaviorContextObjectSerializer.h
    Include/ScriptCanvas/Serialization/DatumSerializer.h
    Include/ScriptCanvas/Serialization/RuntimeVariableSerializer.h
    Include/ScriptCanvas/Translation/Configuration.h
    Include/ScriptCanvas/Translation/GraphToLua.h
    Include/ScriptCanvas/Translation/GraphToLuaUtility.h
    Include/ScriptCanvas/Translation/GraphToX.h
    Include/ScriptCanvas/Translation/Translation.h
    Include/ScriptCanvas/Translation/TranslationContext.h
    Include/ScriptCanvas/Translation/TranslationResult.h
    Include/ScriptCanvas/Translation/TranslationUtilities.h
    Include/ScriptCanvas/Variable/VariableBus.h
    Include/ScriptCanvas/Variable/GraphVariable.h
    Include/ScriptCanvas/Variable/GraphVariableManagerComponent.h
    Include/ScriptCanvas/Variable/VariableCore.h
    Include/ScriptCanvas/Variable/VariableData.h
    Include/ScriptCanvas/Utils/DataUtils.h
    Include/ScriptCanvas/Utils/NodeUtils.h
    Include/ScriptCanvas/Utils/SerializationUtils.h
    Include/ScriptCanvas/Utils/VersionConverters.h
    Include/ScriptCanvas/Utils/VersioningUtils.h
    Include/ScriptCanvas/Utils/BehaviorContextUtils.h
    Include/ScriptCanvas/Utils/ScriptCanvasConstants.h
)

set(SKIP_UNITY_BUILD_INCLUSION_FILES
    Include/ScriptCanvas/Libraries/Core/FunctionCallNode.h
    Include/ScriptCanvas/Libraries/Core/FunctionCallNodeIsOutOfDate.h
)
