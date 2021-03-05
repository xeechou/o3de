/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <AzToolsFramework/ToolsComponents/EditorComponentBase.h>
#include <AzToolsFramework/ToolsComponents/EditorVisibilityBus.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/Serialization/EditContextConstants.inl>

namespace AzToolsFramework
{
    namespace Components
    {
        /** EditorComponentAdapter is a utility base class that provides a consistent pattern for implementing components
        that operate in the editor but may need to share code in different contexts like the launcher.

        EditorComponentAdapter achieves this by delegating to a controller class that implements common behavior instead of
        duplicating code between multiple components.

        To use the EditorComponentAdapter, 3 classes are required:
         - a class that implements the functions required for TController (see below)
         - a configuration struct/class which extends AZ::ComponentConfig
         - A runtime component that will be generated by the editor comoinent on export

         The concrete component extends the adapter and implements behavior which is unique to the component.

        TController can handle any common functionality between the runtime and editor component and is where most of the code for the
        component will live

        TConfiguration is where any data that needs to be serialized out should live.

        TController must implement certain functions to conform to the template. These functions mirror those in
        AZ::Component and must be accesible to any adapter that follows this pattern:

        @code
            static void Reflect(AZ::ReflectContext* context);
            void Activate(EntityId entityId);
            void Deactivate();
            void SetConfiguration(const ComponentConfigurationType& config);
            const ComponentConfigurationType& GetConfiguration() const;
        @endcode

        In addition, certain functions will optionally be called if they are available:

        @code
            static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services);
            static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services);
            static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services);
            void Init();
        @endcode

        It is recommended that TController handle the SerializeContext, but the editor components handle
        the EditContext. TController can friend itself to the editor component to make this work if required.
    */
        template<typename TController, typename TRuntimeComponent, typename TConfiguration = AZ::ComponentConfig>
        class EditorComponentAdapter
            : public EditorComponentBase
        {
        public:

            AZ_RTTI((EditorComponentAdapter, "{2F5A3669-FFE9-4CD7-B9E2-7FC8100CF1A2}", TController, TRuntimeComponent, TConfiguration), EditorComponentBase);

            EditorComponentAdapter() = default;
            EditorComponentAdapter(const TConfiguration& configuration);

            static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services);
            static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services);
            static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services);
            static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& services);

            // EditorComponentBase overrides ...
            void Init() override;
            void Activate() override;
            void Deactivate() override;
            void BuildGameEntity(AZ::Entity* gameEntity) override;

        protected:

            static void Reflect(AZ::ReflectContext* context);

            // AZ::Component overrides ...
            bool ReadInConfig(const AZ::ComponentConfig* baseConfig) override;
            bool WriteOutConfig(AZ::ComponentConfig* outBaseConfig) const override;

            // general configuration change handler invoked with any property edit
            virtual AZ::u32 OnConfigurationChanged();

            // determine if the controller should be activated with the editor component or configuration changes
            virtual bool ShouldActivateController() const;

            TController m_controller;
        };
    } // namespace Components
} // namespace AzToolsFramework

#include "EditorComponentAdapter.inl"
