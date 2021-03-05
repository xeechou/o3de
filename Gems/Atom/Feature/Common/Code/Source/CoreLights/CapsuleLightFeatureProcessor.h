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

#include <Atom/Feature/CoreLights/CapsuleLightFeatureProcessorInterface.h>
#include <Atom/Feature/Utils/GpuBufferHandler.h>
#include <CoreLights/IndexedDataVector.h>
#include <Atom/Feature/CoreLights/PhotometricValue.h>

namespace AZ
{
    class Vector3;
    class Color;

    namespace Render
    {
        class CapsuleLightFeatureProcessor final
            : public CapsuleLightFeatureProcessorInterface
        {
        public:
            AZ_RTTI(AZ::Render::CapsuleLightFeatureProcessor, "{0FC290C5-DD28-4194-8C0B-B90C3291BAF6}", AZ::Render::CapsuleLightFeatureProcessorInterface);

            static void Reflect(AZ::ReflectContext* context);

            CapsuleLightFeatureProcessor();
            virtual ~CapsuleLightFeatureProcessor() = default;

            // FeatureProcessor overrides ...
            void Activate() override;
            void Deactivate() override;
            void Simulate(const SimulatePacket & packet) override;
            void Render(const RenderPacket & packet) override;

            // CapsuleLightFeatureProcessorInterface overrides ...
            LightHandle AcquireLight() override;
            bool ReleaseLight(LightHandle& handle) override;
            LightHandle CloneLight(LightHandle handle) override;
            void SetRgbIntensity(LightHandle handle, const PhotometricColor<PhotometricUnitType>& lightColor) override;
            void SetAttenuationRadius(LightHandle handle, float attenuationRadius) override;
            void SetCapsuleLineSegment(LightHandle handle, const Vector3& startPoint, const Vector3& endPoint) override;
            void SetCapsuleRadius(LightHandle handle, float radius) override;
            void SetCapsuleData(LightHandle handle, const CapsuleLightData& data) override;

            const Data::Instance<RPI::Buffer> GetLightBuffer()const;
            uint32_t GetLightCount()const;

        private:
            CapsuleLightFeatureProcessor(const CapsuleLightFeatureProcessor&) = delete;

            static constexpr const char* FeatureProcessorName = "CapsuleLightFeatureProcessor";

            IndexedDataVector<CapsuleLightData> m_capsuleLightData;
            GpuBufferHandler m_lightBufferHandler;
            bool m_deviceBufferNeedsUpdate = false;
        };
    } // namespace Render
} // namespace AZ
