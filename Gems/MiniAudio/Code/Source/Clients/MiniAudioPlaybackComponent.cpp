/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include "MiniAudioPlaybackComponent.h"

#include <AzCore/Asset/AssetSerializer.h>
#include <AzCore/Serialization/EditContext.h>
#include <MiniAudio/SoundAssetRef.h>

namespace MiniAudio
{
    AZ::ComponentDescriptor* MiniAudioPlaybackComponent_CreateDescriptor()
    {
        return MiniAudioPlaybackComponent::CreateDescriptor();
    }

    AZ::TypeId MiniAudioPlaybackComponent_GetUUID()
    {
        return azrtti_typeid<MiniAudioPlaybackComponent>();
    }

    MiniAudioPlaybackComponent::MiniAudioPlaybackComponent(const MiniAudioPlaybackComponentConfig& config)
        : BaseClass(config)
    {
    }

    void MiniAudioPlaybackComponent::Reflect(AZ::ReflectContext* context)
    {
        BaseClass::Reflect(context);

        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<MiniAudioPlaybackComponent, BaseClass>()
                ->Version(1);
        }

        if (auto behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
        {
            behaviorContext->ConstantProperty("MiniAudioPlaybackComponentTypeId",
                BehaviorConstant(AZ::Uuid(MiniAudioPlaybackComponentTypeId)))
                ->Attribute(AZ::Script::Attributes::Module, "MiniAudio")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common);

            behaviorContext->EBus<MiniAudioPlaybackRequestBus>("MiniAudioPlaybackRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "audio")
                ->Attribute(AZ::Script::Attributes::Category, "MiniAudio Playback")
                ->Event("Play", &MiniAudioPlaybackRequests::Play)
                ->Event("Stop", &MiniAudioPlaybackRequests::Stop)
                ->Event("SetVolume", &MiniAudioPlaybackRequests::SetVolume)
                ->Event("SetLooping", &MiniAudioPlaybackRequests::SetLooping)
                ->Event("IsLooping", &MiniAudioPlaybackRequests::IsLooping)
                ->Event("SetSoundAsset", &MiniAudioPlaybackRequests::SetSoundAssetRef)
                ->Event("GetSoundAsset", &MiniAudioPlaybackRequests::GetSoundAssetRef)
                ->Event("Get Inner Cone Angle In Radians", &MiniAudioPlaybackRequests::GetInnerAngleInRadians)
                ->Event("Set Inner Cone Angle In Radians", &MiniAudioPlaybackRequests::SetInnerAngleInRadians)
                ->Event("Get Inner Cone Angle In Degrees", &MiniAudioPlaybackRequests::GetInnerAngleInDegrees)
                ->Event("Set Inner Cone Angle In Degrees", &MiniAudioPlaybackRequests::SetInnerAngleInDegrees)
                ->Event("Get Outer Cone Angle In Radians", &MiniAudioPlaybackRequests::GetOuterAngleInRadians)
                ->Event("Set Outer Cone Angle In Radians", &MiniAudioPlaybackRequests::SetOuterAngleInRadians)
                ->Event("Get Outer Cone Angle In Degrees", &MiniAudioPlaybackRequests::GetOuterAngleInDegrees)
                ->Event("Set Outer Cone Angle In Degrees", &MiniAudioPlaybackRequests::SetOuterAngleInDegrees)
                ->Event("Get Percent Volume Outside Outer Cone", &MiniAudioPlaybackRequests::GetOuterVolume)
                ->Event("Set Percent Volume Outside Outer Cone", &MiniAudioPlaybackRequests::SetOuterVolume)
                ->Event("Get Fixed Direction", &MiniAudioPlaybackRequests::GetFixedDirecion)
                ->Event("Set Fixed Direction", &MiniAudioPlaybackRequests::SetFixedDirecion)
                ->Event("Get Directional Attenuation Factor", &MiniAudioPlaybackRequests::GetDirectionalAttenuationFactor)
                ->Event("Set Directional Attenuation Factor", &MiniAudioPlaybackRequests::SetDirectionalAttenuationFactor)
                ->Event("Get Direction", &MiniAudioPlaybackRequests::GetDirection)
                ->Event("Set Direction", &MiniAudioPlaybackRequests::SetDirection)
            ;

            behaviorContext->Class<MiniAudioPlaybackComponentController>()->RequestBus("MiniAudioPlaybackRequestBus");
        }
    }
} // namespace MiniAudio
