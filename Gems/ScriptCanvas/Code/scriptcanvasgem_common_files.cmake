#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Source/SystemComponent.cpp
    Source/ScriptCanvasCommonGem.cpp
    Source/PerformanceStatistician.cpp
    Source/PerformanceTracker.cpp
    Include/ScriptCanvas/Asset/AssetRegistry.cpp
    Include/ScriptCanvas/Asset/ExecutionLogAsset.cpp
    Include/ScriptCanvas/Asset/RuntimeAsset.cpp
    Include/ScriptCanvas/Asset/RuntimeAssetHandler.cpp
    Include/ScriptCanvas/Asset/SubgraphInterfaceAssetHandler.cpp
    Include/ScriptCanvas/Core/ExecutionNotificationsBus.cpp
    Include/ScriptCanvas/Core/Connection.cpp
    Include/ScriptCanvas/Core/Contract.cpp
    Include/ScriptCanvas/Core/Contracts/MethodOverloadContract.cpp
    Include/ScriptCanvas/Core/Contracts/RestrictedNodeContract.cpp
    Include/ScriptCanvas/Core/Core.cpp
    Include/ScriptCanvas/Core/Datum.cpp
    Include/ScriptCanvas/Core/EBusHandler.cpp
    Include/ScriptCanvas/Core/Endpoint.cpp
    Include/ScriptCanvas/Core/Graph.cpp
    Include/ScriptCanvas/Core/GraphData.cpp
    Include/ScriptCanvas/Core/MethodConfiguration.cpp
    Include/ScriptCanvas/Core/ModifiableDatumView.cpp
    Include/ScriptCanvas/Core/Node.cpp
    Include/ScriptCanvas/Core/Nodeable.cpp
    Include/ScriptCanvas/Core/NodeableNode.cpp
    Include/ScriptCanvas/Core/NodeableNodeOverloaded.cpp
    Include/ScriptCanvas/Core/Slot.cpp
    Include/ScriptCanvas/Core/SlotConfigurations.cpp
    Include/ScriptCanvas/Core/SlotExecutionMap.cpp
    Include/ScriptCanvas/Core/SlotMetadata.cpp
    Include/ScriptCanvas/Core/SubgraphInterface.cpp
    Include/ScriptCanvas/Core/SubgraphInterfaceUtility.cpp
    Include/ScriptCanvas/Translation/GraphToLua.cpp
    Include/ScriptCanvas/Translation/GraphToLuaUtility.cpp
    Include/ScriptCanvas/Translation/GraphToX.cpp
    Include/ScriptCanvas/Translation/Translation.cpp
    Include/ScriptCanvas/Translation/TranslationContext.cpp
    Include/ScriptCanvas/Translation/TranslationResult.cpp
    Include/ScriptCanvas/Translation/TranslationUtilities.cpp
    Include/ScriptCanvas/Core/Contracts/ConnectionLimitContract.cpp
    Include/ScriptCanvas/Core/Contracts/ContractRTTI.cpp
    Include/ScriptCanvas/Core/Contracts/DisallowReentrantExecutionContract.cpp
    Include/ScriptCanvas/Core/Contracts/DisplayGroupConnectedSlotLimitContract.cpp
    Include/ScriptCanvas/Core/Contracts/DynamicTypeContract.cpp
    Include/ScriptCanvas/Core/Contracts/IsReferenceTypeContract.cpp
    Include/ScriptCanvas/Core/Contracts/MathOperatorContract.cpp
    Include/ScriptCanvas/Core/Contracts/SlotTypeContract.cpp
    Include/ScriptCanvas/Core/Contracts/SupportsMethodContract.cpp
    Include/ScriptCanvas/Core/Contracts/TypeContract.cpp
    Include/ScriptCanvas/Data/BehaviorContextObject.cpp
    Include/ScriptCanvas/Data/BehaviorContextObjectPtr.cpp
    Include/ScriptCanvas/Data/Data.cpp
    Include/ScriptCanvas/Data/DataRegistry.cpp
    Include/ScriptCanvas/Deprecated/VariableDatumBase.cpp
    Include/ScriptCanvas/Deprecated/VariableDatum.cpp
    Include/ScriptCanvas/Deprecated/VariableHelpers.cpp
    Include/ScriptCanvas/Execution/ExecutionContext.cpp
    Include/ScriptCanvas/Execution/ExecutionObjectCloning.cpp
    Include/ScriptCanvas/Execution/ExecutionPerformanceTimer.cpp
    Include/ScriptCanvas/Execution/ExecutionState.cpp
    Include/ScriptCanvas/Execution/RuntimeComponent.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedAPI.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedCloningAPI.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedDebugAPI.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedEBusAPI.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionInterpretedOut.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpreted.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpretedPerActivation.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpretedPure.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpretedSingleton.cpp
    Include/ScriptCanvas/Execution/Interpreted/ExecutionStateInterpretedUtility.cpp
    Include/ScriptCanvas/Grammar/AbstractCodeModel.cpp
    Include/ScriptCanvas/Grammar/DebugMap.cpp
    Include/ScriptCanvas/Grammar/ExecutionTraversalListeners.cpp
    Include/ScriptCanvas/Grammar/ParsingMetaData.cpp
    Include/ScriptCanvas/Grammar/ParsingUtilities.cpp
    Include/ScriptCanvas/Grammar/Primitives.cpp
    Include/ScriptCanvas/Grammar/PrimitivesDeclarations.cpp
    Include/ScriptCanvas/Grammar/PrimitivesExecution.cpp
    Include/ScriptCanvas/Internal/Nodeables/BaseTimer.cpp
    Include/ScriptCanvas/Internal/Nodes/BaseTimerNode.cpp
    Include/ScriptCanvas/Internal/Nodes/ExpressionNodeBase.cpp
    Include/ScriptCanvas/Internal/Nodes/StringFormatted.cpp
    Include/ScriptCanvas/Libraries/Libraries.cpp
    Include/ScriptCanvas/Libraries/Core/AzEventHandler.cpp
    Include/ScriptCanvas/Libraries/Core/BinaryOperator.cpp
    Include/ScriptCanvas/Libraries/Core/CoreNodes.cpp
    Include/ScriptCanvas/Libraries/Core/EBusEventHandler.cpp
    Include/ScriptCanvas/Libraries/Core/ExtractProperty.cpp
    Include/ScriptCanvas/Libraries/Core/EventHandlerTranslationUtility.cpp
    Include/ScriptCanvas/Libraries/Core/ForEach.cpp
    Include/ScriptCanvas/Libraries/Core/FunctionCallNode.cpp
    Include/ScriptCanvas/Libraries/Core/FunctionCallNodeIsOutOfDate.cpp
    Include/ScriptCanvas/Libraries/Core/FunctionDefinitionNode.cpp
    Include/ScriptCanvas/Libraries/Core/GetVariable.cpp
    Include/ScriptCanvas/Libraries/Core/Method.cpp
    Include/ScriptCanvas/Libraries/Core/MethodOverloaded.cpp
    Include/ScriptCanvas/Libraries/Core/Nodeling.cpp
    Include/ScriptCanvas/Libraries/Core/ReceiveScriptEvent.cpp
    Include/ScriptCanvas/Libraries/Core/Repeater.cpp
    Include/ScriptCanvas/Libraries/Core/RepeaterNodeable.cpp
    Include/ScriptCanvas/Libraries/Core/ScriptEventBase.cpp
    Include/ScriptCanvas/Libraries/Core/SendScriptEvent.cpp
    Include/ScriptCanvas/Libraries/Core/SetVariable.cpp
    Include/ScriptCanvas/Libraries/Core/UnaryOperator.cpp
    Include/ScriptCanvas/Libraries/Entity/Entity.cpp
    Include/ScriptCanvas/Libraries/Entity/RotateMethod.cpp
    Include/ScriptCanvas/Libraries/Logic/Any.cpp
    Include/ScriptCanvas/Libraries/Logic/Break.cpp
    Include/ScriptCanvas/Libraries/Logic/Cycle.cpp
    Include/ScriptCanvas/Libraries/Logic/Gate.cpp
    Include/ScriptCanvas/Libraries/Logic/IsNull.cpp
    Include/ScriptCanvas/Libraries/Logic/Logic.cpp
    Include/ScriptCanvas/Libraries/Logic/Once.cpp
    Include/ScriptCanvas/Libraries/Logic/OrderedSequencer.cpp
    Include/ScriptCanvas/Libraries/Logic/Sequencer.cpp
    Include/ScriptCanvas/Libraries/Logic/TargetedSequencer.cpp
    Include/ScriptCanvas/Libraries/Logic/WeightedRandomSequencer.cpp
    Include/ScriptCanvas/Libraries/Logic/While.cpp
    Include/ScriptCanvas/Libraries/Math/Math.cpp
    Include/ScriptCanvas/Libraries/Math/MathExpression.cpp
    Include/ScriptCanvas/Libraries/Math/MathNodeUtilities.cpp
    Include/ScriptCanvas/Libraries/Comparison/Comparison.cpp
    Include/ScriptCanvas/Libraries/Time/Time.cpp
    Include/ScriptCanvas/Libraries/Time/Countdown.cpp
    Include/ScriptCanvas/Libraries/Time/DelayNodeable.cpp
    Include/ScriptCanvas/Libraries/Time/Duration.cpp
    Include/ScriptCanvas/Libraries/Time/HeartBeat.cpp
    Include/ScriptCanvas/Libraries/Time/Timer.cpp
    Include/ScriptCanvas/Libraries/Time/TimeDelayNodeable.cpp
    Include/ScriptCanvas/Libraries/Time/DurationNodeable.cpp
    Include/ScriptCanvas/Libraries/Time/HeartBeatNodeable.cpp
    Include/ScriptCanvas/Libraries/Time/TimerNodeable.cpp
    Include/ScriptCanvas/Libraries/Spawning/Spawning.cpp
    Include/ScriptCanvas/Libraries/Spawning/SpawnableAsset.cpp
    Include/ScriptCanvas/Libraries/Spawning/SpawnTicketInstance.cpp
    Include/ScriptCanvas/Libraries/Spawning/CreateSpawnTicketNodeable.cpp
    Include/ScriptCanvas/Libraries/Spawning/SpawnNodeable.cpp
    Include/ScriptCanvas/Libraries/Spawning/DespawnNodeable.cpp
    Include/ScriptCanvas/Libraries/String/Contains.cpp
    Include/ScriptCanvas/Libraries/String/Replace.cpp
    Include/ScriptCanvas/Libraries/String/String.cpp
    Include/ScriptCanvas/Libraries/String/StringMethods.cpp
    Include/ScriptCanvas/Libraries/String/Utilities.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/ExpectEqual.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/ExpectFalse.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/ExpectGreaterThan.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/ExpectGreaterThanEqual.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/ExpectLessThan.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/ExpectLessThanEqual.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/ExpectNotEqual.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/ExpectTrue.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/UnitTesting.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/UnitTestingLibrary.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/Auxiliary/Auxiliary.cpp
    Include/ScriptCanvas/Libraries/UnitTesting/UnitTestBusSender.cpp
    Include/ScriptCanvas/Libraries/Operators/Operators.cpp
    Include/ScriptCanvas/Libraries/Operators/Operator.cpp
    Include/ScriptCanvas/Libraries/Operators/Containers/OperatorAt.cpp
    Include/ScriptCanvas/Libraries/Operators/Containers/OperatorBack.cpp
    Include/ScriptCanvas/Libraries/Operators/Containers/OperatorClear.cpp
    Include/ScriptCanvas/Libraries/Operators/Containers/OperatorEmpty.cpp
    Include/ScriptCanvas/Libraries/Operators/Containers/OperatorErase.cpp
    Include/ScriptCanvas/Libraries/Operators/Containers/OperatorFront.cpp
    Include/ScriptCanvas/Libraries/Operators/Containers/OperatorInsert.cpp
    Include/ScriptCanvas/Libraries/Operators/Containers/OperatorPushBack.cpp
    Include/ScriptCanvas/Libraries/Operators/Containers/OperatorSize.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorAdd.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorArithmetic.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorDiv.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorDivideByNumber.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorLength.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorLerp.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorMul.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorSub.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorLerpNodeable.cpp
    Include/ScriptCanvas/Libraries/Operators/Math/OperatorLerpNodeableNode.cpp
    Include/ScriptCanvas/Serialization/BehaviorContextObjectSerializer.cpp
    Include/ScriptCanvas/Serialization/DatumSerializer.cpp
    Include/ScriptCanvas/Serialization/RuntimeVariableSerializer.cpp
    Include/ScriptCanvas/Data/DataTrait.cpp
    Include/ScriptCanvas/Data/PropertyTraits.cpp
    Include/ScriptCanvas/Variable/GraphVariable.cpp
    Include/ScriptCanvas/Variable/GraphVariableManagerComponent.cpp
    Include/ScriptCanvas/Variable/VariableCore.cpp
    Include/ScriptCanvas/Variable/VariableData.cpp
    Include/ScriptCanvas/Utils/DataUtils.cpp
    Include/ScriptCanvas/Utils/NodeUtils.cpp
    Include/ScriptCanvas/Utils/VersionConverters.cpp
    Include/ScriptCanvas/Utils/VersioningUtils.cpp
    Include/ScriptCanvas/Utils/BehaviorContextUtils.cpp
)
