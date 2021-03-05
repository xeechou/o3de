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

#include <Atom/Component/DebugCamera/CameraControllerComponent.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace AZ
{
    namespace Debug
    {
        void CameraControllerComponent::Reflect(AZ::ReflectContext* context)
        {
            if (auto* serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
            {
                serializeContext->Class<CameraControllerComponent, AZ::Component>()
                    ->Version(1);
            }
        }

        void CameraControllerComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
        {
            required.push_back(AZ_CRC("TransformService", 0x8ee22c50));
            required.push_back(AZ_CRC("CameraService", 0x1dd1caa4));
        }
            
        void CameraControllerComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
        {
            provided.push_back(AZ_CRC("CameraControllerService", 0xc35788f9));
        }

        void CameraControllerComponent::Enable(TypeId typeId)
        {
            // Enable this controller if type id matches, otherwise disable this controller
            if (typeId == RTTI_GetType())
            {
                if (!m_enabled)
                {
                    m_enabled = true;
                    OnEnabled();
                }
            }
            else
            {
                if (m_enabled)
                {
                    m_enabled = false;
                    OnDisabled();
                }
            }
        }

        void CameraControllerComponent::Disable()
        {
            if (m_enabled)
            {
                m_enabled = false;
                OnDisabled();
            }
        }

        void CameraControllerComponent::Reset()
        {
            if (m_enabled)
            {
                OnDisabled();
                OnEnabled();
            }
        }
        
        void CameraControllerComponent::Activate()
        {
            CameraControllerRequestBus::Handler::BusConnect(GetEntityId());
        }

        void CameraControllerComponent::Deactivate()
        {
            if (m_enabled)
            {
                m_enabled = false;
                OnDisabled();
            }
            CameraControllerRequestBus::Handler::BusDisconnect();
        }
        
    } // namespace Debug
} // namespace AZ
