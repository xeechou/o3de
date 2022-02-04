/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Interface/Interface.h>
#include <Multiplayer/Session/ISessionRequests.h>
#include <Multiplayer/Session/ISessionHandlingRequests.h>
#include <Multiplayer/Session/IMatchmakingRequests.h>
#include <Multiplayer/Session/MatchmakingNotifications.h>
#include <AzTest/AzTest.h>

#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/utils/Outcome.h>
#include <aws/gamelift/GameLiftClient.h>
#include <aws/gamelift/GameLiftErrors.h>
#include <aws/gamelift/model/AcceptMatchRequest.h>
#include <aws/gamelift/model/AcceptMatchResult.h>
#include <aws/gamelift/model/CreateGameSessionRequest.h>
#include <aws/gamelift/model/CreateGameSessionResult.h>
#include <aws/gamelift/model/CreatePlayerSessionRequest.h>
#include <aws/gamelift/model/CreatePlayerSessionResult.h>
#include <aws/gamelift/model/DescribeMatchmakingRequest.h>
#include <aws/gamelift/model/DescribeMatchmakingResult.h>
#include <aws/gamelift/model/SearchGameSessionsRequest.h>
#include <aws/gamelift/model/SearchGameSessionsResult.h>
#include <aws/gamelift/model/StartGameSessionPlacementRequest.h>
#include <aws/gamelift/model/StartGameSessionPlacementResult.h>
#include <aws/gamelift/model/StartMatchmakingRequest.h>
#include <aws/gamelift/model/StartMatchmakingResult.h>
#include <aws/gamelift/model/StopMatchmakingRequest.h>
#include <aws/gamelift/model/StopMatchmakingResult.h>

using namespace Aws::GameLift;

class GameLiftClientMock
    : public GameLiftClient
{
public:
    GameLiftClientMock()
        : GameLiftClient(Aws::Auth::AWSCredentials())
    {
    }

    MOCK_CONST_METHOD1(AcceptMatch, Model::AcceptMatchOutcome(const Model::AcceptMatchRequest&));
    MOCK_CONST_METHOD1(CreateGameSession, Model::CreateGameSessionOutcome(const Model::CreateGameSessionRequest&));
    MOCK_CONST_METHOD1(CreatePlayerSession, Model::CreatePlayerSessionOutcome(const Model::CreatePlayerSessionRequest&));
    MOCK_CONST_METHOD1(DescribeMatchmaking, Model::DescribeMatchmakingOutcome(const Model::DescribeMatchmakingRequest&));
    MOCK_CONST_METHOD1(SearchGameSessions, Model::SearchGameSessionsOutcome(const Model::SearchGameSessionsRequest&));
    MOCK_CONST_METHOD1(StartGameSessionPlacement, Model::StartGameSessionPlacementOutcome(const Model::StartGameSessionPlacementRequest&));
    MOCK_CONST_METHOD1(StartMatchmaking, Model::StartMatchmakingOutcome(const Model::StartMatchmakingRequest&));
    MOCK_CONST_METHOD1(StopMatchmaking, Model::StopMatchmakingOutcome(const Model::StopMatchmakingRequest&));
};

class MatchmakingAsyncRequestNotificationsHandlerMock
    : public Multiplayer::MatchmakingAsyncRequestNotificationBus::Handler
{
public:
    MatchmakingAsyncRequestNotificationsHandlerMock()
    {
        Multiplayer::MatchmakingAsyncRequestNotificationBus::Handler::BusConnect();
    }

    ~MatchmakingAsyncRequestNotificationsHandlerMock()
    {
        Multiplayer::MatchmakingAsyncRequestNotificationBus::Handler::BusDisconnect();
    }

    MOCK_METHOD0(OnAcceptMatchAsyncComplete, void());
    MOCK_METHOD1(OnStartMatchmakingAsyncComplete, void(const AZStd::string&));
    MOCK_METHOD0(OnStopMatchmakingAsyncComplete, void());
};

class MatchmakingNotificationsHandlerMock
    : public Multiplayer::MatchmakingNotificationBus::Handler
{
public:
    MatchmakingNotificationsHandlerMock()
    {
        Multiplayer::MatchmakingNotificationBus::Handler::BusConnect();
    }

    ~MatchmakingNotificationsHandlerMock()
    {
        Multiplayer::MatchmakingNotificationBus::Handler::BusDisconnect();
    }

    void OnMatchAcceptance() override
    {
        ++m_numMatchAcceptance;
    }

    void OnMatchComplete() override
    {
        ++m_numMatchComplete;
    }

    void OnMatchError() override
    {
        ++m_numMatchError;
    }

    void OnMatchFailure() override
    {
        ++m_numMatchFailure;
    }

    int m_numMatchAcceptance = 0;
    int m_numMatchComplete = 0;
    int m_numMatchError = 0;
    int m_numMatchFailure = 0;
};

class SessionAsyncRequestNotificationsHandlerMock
    : public Multiplayer::SessionAsyncRequestNotificationBus::Handler
{
public:
    SessionAsyncRequestNotificationsHandlerMock()
    {
        Multiplayer::SessionAsyncRequestNotificationBus::Handler::BusConnect();
    }

    ~SessionAsyncRequestNotificationsHandlerMock()
    {
        Multiplayer::SessionAsyncRequestNotificationBus::Handler::BusDisconnect();
    }

    MOCK_METHOD1(OnCreateSessionAsyncComplete, void(const AZStd::string&));
    MOCK_METHOD1(OnSearchSessionsAsyncComplete, void(const Multiplayer::SearchSessionsResponse&));
    MOCK_METHOD1(OnJoinSessionAsyncComplete, void(bool));
    MOCK_METHOD0(OnLeaveSessionAsyncComplete, void());
};

class SessionHandlingClientRequestsMock
    : public Multiplayer::ISessionHandlingClientRequests
{
public:
    SessionHandlingClientRequestsMock()
    {
        AZ::Interface<Multiplayer::ISessionHandlingClientRequests>::Register(this);
    }

    virtual ~SessionHandlingClientRequestsMock()
    {
        AZ::Interface<Multiplayer::ISessionHandlingClientRequests>::Unregister(this);
    }

    MOCK_METHOD1(RequestPlayerJoinSession, bool(const Multiplayer::SessionConnectionConfig&));
    MOCK_METHOD0(RequestPlayerLeaveSession, void());
};
