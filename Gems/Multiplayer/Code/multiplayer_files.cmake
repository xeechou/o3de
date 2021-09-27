#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

set(FILES
    Include/Multiplayer/IMultiplayer.h
    Include/Multiplayer/IMultiplayerDebug.h
    Include/Multiplayer/IMultiplayerTools.h
    Include/Multiplayer/MultiplayerConstants.h
    Include/Multiplayer/MultiplayerStats.h
    Include/Multiplayer/MultiplayerTypes.h
    Include/Multiplayer/Components/LocalPredictionPlayerInputComponent.h
    Include/Multiplayer/Components/MultiplayerComponent.h
    Include/Multiplayer/Components/MultiplayerComponentRegistry.h
    Include/Multiplayer/Components/MultiplayerController.h
    Include/Multiplayer/Components/NetBindComponent.h
    Include/Multiplayer/Components/NetworkHierarchyChildComponent.h
    Include/Multiplayer/Components/NetworkHierarchyRootComponent.h
    Include/Multiplayer/Components/NetworkHierarchyBus.h
    Include/Multiplayer/Components/NetworkCharacterComponent.h
    Include/Multiplayer/Components/NetworkHitVolumesComponent.h
    Include/Multiplayer/Components/NetworkRigidBodyComponent.h
    Include/Multiplayer/Components/NetworkTransformComponent.h
    Include/Multiplayer/ConnectionData/IConnectionData.h
    Include/Multiplayer/EntityDomains/IEntityDomain.h
    Include/Multiplayer/IMultiplayer.h
    Include/Multiplayer/IMultiplayerTools.h
    Include/Multiplayer/INetworkSpawnableLibrary.h
    Include/Multiplayer/MultiplayerConstants.h
    Include/Multiplayer/MultiplayerStats.h
    Include/Multiplayer/MultiplayerTypes.h
    Include/Multiplayer/NetworkEntity/EntityReplication/ReplicationRecord.h
    Include/Multiplayer/NetworkEntity/IFilterEntityManager.h
    Include/Multiplayer/NetworkEntity/INetworkEntityManager.h
    Include/Multiplayer/NetworkEntity/NetworkEntityHandle.h
    Include/Multiplayer/NetworkEntity/NetworkEntityHandle.inl
    Include/Multiplayer/NetworkEntity/NetworkEntityRpcMessage.h
    Include/Multiplayer/NetworkEntity/NetworkEntityUpdateMessage.h
    Include/Multiplayer/NetworkInput/IMultiplayerComponentInput.h
    Include/Multiplayer/NetworkInput/NetworkInput.h
    Include/Multiplayer/NetworkTime/INetworkTime.h
    Include/Multiplayer/NetworkTime/RewindableArray.h
    Include/Multiplayer/NetworkTime/RewindableArray.inl
    Include/Multiplayer/NetworkTime/RewindableFixedVector.h
    Include/Multiplayer/NetworkTime/RewindableFixedVector.inl
    Include/Multiplayer/NetworkTime/RewindableObject.h
    Include/Multiplayer/NetworkTime/RewindableObject.inl
    Include/Multiplayer/Physics/PhysicsUtils.h
    Include/Multiplayer/ReplicationWindows/IReplicationWindow.h
    Source/AutoGen/AutoComponentTypes_Header.jinja
    Source/AutoGen/AutoComponentTypes_Source.jinja
    Source/AutoGen/AutoComponent_Common.jinja
    Source/AutoGen/AutoComponent_Header.jinja
    Source/AutoGen/AutoComponent_Source.jinja
    Source/AutoGen/LocalPredictionPlayerInputComponent.AutoComponent.xml
    Source/AutoGen/Multiplayer.AutoPackets.xml
    Source/AutoGen/MultiplayerEditor.AutoPackets.xml
    Source/AutoGen/NetworkCharacterComponent.AutoComponent.xml
    Source/AutoGen/NetworkConnectionComponent.AutoComponent.xml
    Source/AutoGen/NetworkHitVolumesComponent.AutoComponent.xml
    Source/AutoGen/NetworkRigidBodyComponent.AutoComponent.xml
    Source/AutoGen/NetworkTransformComponent.AutoComponent.xml
    Source/AutoGen/NetworkHierarchyChildComponent.AutoComponent.xml
    Source/AutoGen/NetworkHierarchyRootComponent.AutoComponent.xml
    Source/Components/LocalPredictionPlayerInputComponent.cpp
    Source/Components/MultiplayerComponent.cpp
    Source/Components/MultiplayerComponentRegistry.cpp
    Source/Components/MultiplayerController.cpp
    Source/Components/NetBindComponent.cpp
    Source/Components/NetworkHierarchyChildComponent.cpp
    Source/Components/NetworkHierarchyRootComponent.cpp
    Source/Components/NetworkCharacterComponent.cpp
    Source/Components/NetworkHitVolumesComponent.cpp
    Source/Components/NetworkRigidBodyComponent.cpp
    Source/Components/NetworkTransformComponent.cpp
    Source/ConnectionData/ClientToServerConnectionData.cpp
    Source/ConnectionData/ClientToServerConnectionData.h
    Source/ConnectionData/ClientToServerConnectionData.inl
    Source/ConnectionData/ServerToClientConnectionData.cpp
    Source/ConnectionData/ServerToClientConnectionData.h
    Source/ConnectionData/ServerToClientConnectionData.inl
    Source/Editor/MultiplayerEditorConnection.cpp
    Source/Editor/MultiplayerEditorConnection.h
    Source/EntityDomains/FullOwnershipEntityDomain.cpp
    Source/EntityDomains/FullOwnershipEntityDomain.h
    Source/MultiplayerStats.cpp
    Source/MultiplayerSystemComponent.cpp
    Source/MultiplayerSystemComponent.h
    Source/NetworkEntity/EntityReplication/EntityReplicationManager.cpp
    Source/NetworkEntity/EntityReplication/EntityReplicator.cpp
    Source/NetworkEntity/EntityReplication/PropertyPublisher.cpp
    Source/NetworkEntity/EntityReplication/PropertyPublisher.h
    Source/NetworkEntity/EntityReplication/PropertySubscriber.cpp
    Source/NetworkEntity/EntityReplication/PropertySubscriber.h
    Source/NetworkEntity/EntityReplication/ReplicationRecord.cpp
    Source/NetworkEntity/NetworkEntityAuthorityTracker.cpp
    Source/NetworkEntity/NetworkEntityAuthorityTracker.h
    Source/NetworkEntity/NetworkEntityHandle.cpp
    Source/NetworkEntity/NetworkEntityManager.cpp
    Source/NetworkEntity/NetworkEntityManager.h
    Source/NetworkEntity/NetworkEntityRpcMessage.cpp
    Source/NetworkEntity/NetworkEntityTracker.cpp
    Source/NetworkEntity/NetworkEntityTracker.h
    Source/NetworkEntity/NetworkEntityTracker.inl
    Source/NetworkEntity/NetworkEntityUpdateMessage.cpp
    Source/NetworkEntity/NetworkSpawnableLibrary.cpp
    Source/NetworkEntity/NetworkSpawnableLibrary.h
    Source/NetworkInput/NetworkInput.cpp
    Source/NetworkInput/NetworkInputArray.cpp
    Source/NetworkInput/NetworkInputArray.h
    Source/NetworkInput/NetworkInputChild.cpp
    Source/NetworkInput/NetworkInputChild.h
    Source/NetworkInput/NetworkInputHistory.cpp
    Source/NetworkInput/NetworkInputHistory.h
    Source/NetworkInput/NetworkInputMigrationVector.cpp
    Source/NetworkInput/NetworkInputMigrationVector.h
    Source/NetworkTime/NetworkTime.cpp
    Source/NetworkTime/NetworkTime.h
    Source/Pipeline/NetworkSpawnableHolderComponent.cpp
    Source/Pipeline/NetworkSpawnableHolderComponent.h
    Source/ReplicationWindows/NullReplicationWindow.cpp
    Source/ReplicationWindows/NullReplicationWindow.h
    Source/ReplicationWindows/ServerToClientReplicationWindow.cpp
    Source/ReplicationWindows/ServerToClientReplicationWindow.h
)
