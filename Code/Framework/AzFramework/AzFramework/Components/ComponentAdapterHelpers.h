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

#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityBus.h>

namespace AzFramework
{
    namespace Components
    {
        //////////////////////////////////////////////////////////////////////////
        // Helper functions to make certain functions optional in the class wrapped by
        // EditorComponentAdapter and ComponentAdapter.

        // Make Init() Optional

        template<typename T, typename = void>
        struct ComponentInitHelper
        {
            static void Init([[maybe_unused]] T& controller)
            {
            }
        };

        template<typename T>
        struct ComponentInitHelper<T, AZStd::void_t<decltype(AZStd::declval<T>().Init())>>
        {
            static void Init(T& controller)
            {
                controller.Init();
            }
        };

        template<typename T, typename = void>
        struct ComponentActivateHelper
        {
            static void Activate([[maybe_unused]] T& controller, [[maybe_unused]] const AZ::EntityComponentIdPair& entityComponentIdPair)
            {
            }
        };

        template<typename T>
        struct ComponentActivateHelper<T, AZStd::void_t<decltype(AZStd::declval<T>().Activate(AZ::EntityId()))>>
        {
            static void Activate(T& controller, const AZ::EntityComponentIdPair& entityComponentIdPair)
            {
                controller.Activate(entityComponentIdPair.GetEntityId());
            }
        };

        template<typename T>
        struct ComponentActivateHelper<T, AZStd::void_t<decltype(AZStd::declval<T>().Activate(AZ::EntityComponentIdPair()))>>
        {
            static void Activate(T& controller, const AZ::EntityComponentIdPair& entityComponentIdPair)
            {
                controller.Activate(entityComponentIdPair);
            }
        };

        // Make GetProvidedServices, GetDependentServicesHelper, GetRequiredServices and GetIncompatibleServices optional.

        template<typename T>
        void GetProvidedServicesHelper(AZ::ComponentDescriptor::DependencyArrayType&, const AZStd::false_type&) {}

        template<typename T>
        void GetProvidedServicesHelper(AZ::ComponentDescriptor::DependencyArrayType& services, const AZStd::true_type&)
        {
            T::GetProvidedServices(services);
        }

        template<typename T>
        void GetDependentServicesHelper(AZ::ComponentDescriptor::DependencyArrayType&, const AZStd::false_type&) {}

        template<typename T>
        void GetDependentServicesHelper(AZ::ComponentDescriptor::DependencyArrayType& services, const AZStd::true_type&)
        {
            T::GetDependentServices(services);
        }

        template<typename T>
        void GetRequiredServicesHelper(AZ::ComponentDescriptor::DependencyArrayType&, const AZStd::false_type&) {}

        template<typename T>
        void GetRequiredServicesHelper(AZ::ComponentDescriptor::DependencyArrayType& services, const AZStd::true_type&)
        {
            T::GetRequiredServices(services);
        }

        template<typename T>
        void GetIncompatibleServicesHelper(AZ::ComponentDescriptor::DependencyArrayType&, const AZStd::false_type&) {}

        template<typename T>
        void GetIncompatibleServicesHelper(AZ::ComponentDescriptor::DependencyArrayType& services, const AZStd::true_type&)
        {
            T::GetIncompatibleServices(services);
        }
    } // namespace Components
} // namespace AzFramework
