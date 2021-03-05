/*
* All or portions of this file Copyright(c) Amazon.com, Inc.or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution(the "License").All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file.Do not
* remove or modify any license notices.This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/IO/Streamer/Streamer.h>
#include <AzCore/Serialization/Json/JsonSerialization.h>
#include <AzCore/Serialization/Json/JsonSystemComponent.h>
#include <AzCore/Serialization/Json/RegistrationContext.h>
#include <AzCore/UnitTest/TestTypes.h>

#include <AzTest/AzTest.h>

namespace UnitTest
{
    /**
     * Unit test fixture for setting up things commonly needed by builders' unit tests
     */
    class BuilderTestFixture
        : public AllocatorsFixture
        // Only used to provide the serialize context for now
        , public AZ::ComponentApplicationBus::Handler
    {
    public:

        virtual void Reflect(AZ::ReflectContext* context);

        // ComponentApplicationMessages overrides...
        AZ::ComponentApplication* GetApplication() override { return nullptr; }
        void RegisterComponentDescriptor(const AZ::ComponentDescriptor*) override { }
        void UnregisterComponentDescriptor(const AZ::ComponentDescriptor*) override { }
        bool AddEntity(AZ::Entity*) override { return false; }
        bool RemoveEntity(AZ::Entity*) override { return false; }
        bool DeleteEntity(const AZ::EntityId&) override { return false; }
        AZ::Entity* FindEntity(const AZ::EntityId&) override { return nullptr; }
        AZ::BehaviorContext*  GetBehaviorContext() override { return nullptr; }
        const char* GetExecutableFolder() const override { return nullptr; }
        const char* GetAppRoot() const override { return nullptr; }
        AZ::Debug::DrillerManager* GetDrillerManager() override { return nullptr; }
        void EnumerateEntities(const EntityCallback& /*callback*/) override {}
        void QueryApplicationType(AZ::ApplicationTypeQuery& /*appType*/) const override {}
        // The functions we need to implement.
        AZ::SerializeContext* GetSerializeContext() override;
        AZ::JsonRegistrationContext* GetJsonRegistrationContext() override;

        // AllocatorsFixture overrides...
        void SetUp() override;
        void TearDown() override;

    protected:
        AZStd::unique_ptr<AZ::SerializeContext> m_context;
        char m_currentDir[AZ_MAX_PATH_LEN];

        // Required for json serializer
        AZStd::unique_ptr<AZ::JsonSystemComponent> m_jsonSystemComponent;
        AZStd::unique_ptr<AZ::JsonRegistrationContext> m_jsonRegistrationContext;
        AZStd::unique_ptr<AZ::IO::Streamer> m_streamer;
    };

}// namespace UnitTest
