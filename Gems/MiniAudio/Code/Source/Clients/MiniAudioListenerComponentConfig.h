/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Asset/AssetCommon.h>
#include <AzCore/Component/ComponentBus.h>
#include <AzCore/RTTI/RTTI.h>
#include <AzCore/Serialization/EditContext.h>

namespace MiniAudio
{
    class MiniAudioListenerComponentConfig final
        : public AZ::ComponentConfig
    {
    public:
        AZ_RTTI(MiniAudioListenerComponentConfig, "{7987E444-3A98-469C-B38B-EDD9C247D7F1}");

        static void Reflect(AZ::ReflectContext* context);

        //! Listener follows the specified entity.
        AZ::EntityId m_followEntity;

        AZ::u32 m_listenerIndex = 0;
    };
} // namespace MiniAudio
