/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AtomToolsFramework/Viewport/ViewportInputBehaviorController/PanCameraBehavior.h>
#include <AtomToolsFramework/Viewport/ViewportInputBehaviorController/ViewportInputBehaviorControllerInterface.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Math/Quaternion.h>
#include <AzCore/Math/Vector3.h>

namespace AtomToolsFramework
{
    PanCameraBehavior::PanCameraBehavior(ViewportInputBehaviorControllerInterface* controller)
        : ViewportInputBehavior(controller)
    {
    }

    void PanCameraBehavior::End()
    {
        float distanceToObject = m_controller->GetDistanceToObject();
        AZ::Transform transform = AZ::Transform::CreateIdentity();
        AZ::TransformBus::EventResult(transform, m_cameraEntityId, &AZ::TransformBus::Events::GetWorldTM);
        AZ::Vector3 objectPosition = transform.GetTranslation() + transform.GetBasisY() * distanceToObject;
        m_controller->SetObjectPosition(objectPosition);
    }

    void PanCameraBehavior::TickInternal(float x, float y, [[maybe_unused]] float z)
    {
        AZ::Transform transform = AZ::Transform::CreateIdentity();
        AZ::TransformBus::EventResult(transform, m_cameraEntityId, &AZ::TransformBus::Events::GetWorldTM);
        AZ::Quaternion rotation = transform.GetRotation();
        const AZ::Vector3 right = transform.GetBasisX();
        rotation =
            AZ::Quaternion::CreateFromAxisAngle(AZ::Vector3::CreateAxisZ(), -x) * AZ::Quaternion::CreateFromAxisAngle(right, -y) * rotation;
        rotation.Normalize();
        AZ::TransformBus::Event(m_cameraEntityId, &AZ::TransformBus::Events::SetWorldRotationQuaternion, rotation);
    }

    float PanCameraBehavior::GetSensitivityX()
    {
        return SensitivityX;
    }

    float PanCameraBehavior::GetSensitivityY()
    {
        return SensitivityY;
    }
} // namespace AtomToolsFramework
