/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "MiniAudioListenerComponentConfig.h"

#include <AzCore/Asset/AssetSerializer.h>

namespace MiniAudio
{
    void MiniAudioListenerComponentConfig::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MiniAudioListenerComponentConfig>()
                ->Version(1)
                ->Field("Follow Entity", &MiniAudioListenerComponentConfig::m_followEntity)
                ->Field("Listener Index", &MiniAudioListenerComponentConfig::m_listenerIndex)
                ->Field("Inner Cone Angle", &MiniAudioListenerComponentConfig::m_innerAngleInDegrees)
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->Attribute(AZ::Edit::Attributes::Step, 1.0f)
                    ->Attribute(AZ::Edit::Attributes::Max, 360.0f)
                    ->Attribute(AZ::Edit::Attributes::Suffix, " degrees")
                ->Field("Outer Cone Angle", &MiniAudioListenerComponentConfig::m_outerAngleInDegrees)
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->Attribute(AZ::Edit::Attributes::Step, 1.0f)
                    ->Attribute(AZ::Edit::Attributes::Max, 360.0f)
                    ->Attribute(AZ::Edit::Attributes::Suffix, " degrees")
                ->Field("Outer Volume", &MiniAudioListenerComponentConfig::m_outerVolume)
                    ->Attribute(AZ::Edit::Attributes::Min, 0.0f)
                    ->Attribute(AZ::Edit::Attributes::Step, 1.0f)
                    ->Attribute(AZ::Edit::Attributes::Max, 100.0f)
                    ->Attribute(AZ::Edit::Attributes::Suffix, " %")
                ;
        }
    }
} // namespace MiniAudio
