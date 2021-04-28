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
#include <AzCore/Component/ComponentBus.h>
#include <GradientSignal/GradientSampler.h>
#include <GradientSignal/Ebuses/DitherGradientRequestBus.h>
#include <GradientSignal/Ebuses/GradientRequestBus.h>
#include <LmbrCentral/Dependency/DependencyMonitor.h>
#include <GradientSignal/Ebuses/SectorDataRequestBus.h>

namespace LmbrCentral
{
    template<typename, typename>
    class EditorWrappedComponentBase;
}

namespace GradientSignal
{
    class DitherGradientConfig
        : public AZ::ComponentConfig
    {
    public:
        AZ_CLASS_ALLOCATOR(DitherGradientConfig, AZ::SystemAllocator, 0);
        AZ_RTTI(DitherGradientConfig, "{8F519317-4E83-4CF0-BEC9-C5F3F3198F20}", AZ::ComponentConfig);
        static void Reflect(AZ::ReflectContext* context);
        bool m_useSystemPointsPerUnit = true;
        float m_pointsPerUnit = 1.0f;
        AZ::Vector3 m_patternOffset = AZ::Vector3::CreateZero();
        enum class BayerPatternType : AZ::u8
        {
            PATTERN_SIZE_4x4 = 4,
            PATTERN_SIZE_8x8 = 8,
        };
        BayerPatternType m_patternType = BayerPatternType::PATTERN_SIZE_4x4;
        GradientSampler m_gradientSampler;
        bool IsPointsPerUnitResdOnly() const;
    };

    static const AZ::Uuid DitherGradientComponentTypeId = "{F69E885E-9D43-480D-A549-E5EE503A8F29}";

    /**
    * calculates a gradient output value by applying ordered dithering to the input gradient value
    */
    class DitherGradientComponent
        : public AZ::Component
        , private GradientRequestBus::Handler
        , private DitherGradientRequestBus::Handler
        , private SectorDataNotificationBus::Handler
    {
    public:
        template<typename, typename> friend class LmbrCentral::EditorWrappedComponentBase;
        AZ_COMPONENT(DitherGradientComponent, DitherGradientComponentTypeId);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& services);
        static void Reflect(AZ::ReflectContext* context);

        DitherGradientComponent(const DitherGradientConfig& configuration);
        DitherGradientComponent() = default;
        ~DitherGradientComponent() = default;

        //////////////////////////////////////////////////////////////////////////
        // AZ::Component interface implementation
        void Activate() override;
        void Deactivate() override;
        bool ReadInConfig(const AZ::ComponentConfig* baseConfig) override;
        bool WriteOutConfig(AZ::ComponentConfig* outBaseConfig) const override;

        //////////////////////////////////////////////////////////////////////////
        // GradientRequestBus
        float GetValue(const GradientSampleParams& sampleParams) const override;
        bool IsEntityInHierarchy(const AZ::EntityId& entityId) const override;

        //////////////////////////////////////////////////////////////////////////
        // SectorDataNotificationBus
        void OnSectorDataConfigurationUpdated() const override;

    protected:

        //////////////////////////////////////////////////////////////////////////
        // DitheredGradientRequestBus
        bool GetUseSystemPointsPerUnit() const override;
        void SetUseSystemPointsPerUnit(bool value) override;

        float GetPointsPerUnit() const override;
        void SetPointsPerUnit(float points) override;

        AZ::Vector3 GetPatternOffset() const override;
        void SetPatternOffset(AZ::Vector3 offset) override;

        AZ::u8 GetPatternType() const override;
        void SetPatternType(AZ::u8 type) override;

        GradientSampler& GetGradientSampler() override;

    private:
        DitherGradientConfig m_configuration;
        LmbrCentral::DependencyMonitor m_dependencyMonitor;
    };
}
