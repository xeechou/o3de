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

#include <AzCore/PlatformDef.h>

#include <AzCore/Memory/SystemAllocator.h>
#include <IGem.h>

#include <AudioEngineWwiseGemSystemComponent.h>

#if defined(AUDIO_ENGINE_WWISE_EDITOR)
    #include <AudioControlBuilderComponent.h>
    #include <WwiseBuilderComponent.h>
#endif // AUDIO_ENGINE_WWISE_EDITOR

namespace AudioEngineWwiseGem
{
    class AudioEngineWwiseModule
        : public CryHooksModule
    {
    public:
        AZ_RTTI(AudioEngineWwiseModule, "{303B0192-D866-4378-9342-728AA6E66F74}", CryHooksModule);
        AZ_CLASS_ALLOCATOR(AudioEngineWwiseModule, AZ::SystemAllocator, 0);

        AudioEngineWwiseModule()
            : CryHooksModule()
        {
            m_descriptors.insert(m_descriptors.end(), {
                AudioEngineWwiseGemSystemComponent::CreateDescriptor(),
            #if defined(AUDIO_ENGINE_WWISE_EDITOR)
                AudioControlBuilder::BuilderPluginComponent::CreateDescriptor(),
                WwiseBuilder::BuilderPluginComponent::CreateDescriptor(),
            #endif // AUDIO_ENGINE_WWISE_EDITOR
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<AudioEngineWwiseGemSystemComponent>(),
            };
        }
    };

} // namespace AudioEngineWwiseGem

// DO NOT MODIFY THIS LINE UNLESS YOU RENAME THE GEM
// The first parameter should be GemName_GemIdLower
// The second should be the fully qualified name of the class above
AZ_DECLARE_MODULE_CLASS(Gem_AudioEngineWwise, AudioEngineWwiseGem::AudioEngineWwiseModule)
