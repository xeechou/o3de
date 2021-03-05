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

#include <Atom/RHI/Device.h>
#include <Atom/RHI/MemoryStatisticsBus.h>

#include <AzCore/Debug/EventTrace.h>
#include <AzCore/std/sort.h>

namespace AZ
{
    namespace RHI
    {
        bool Device::IsInitialized() const
        {
            return m_physicalDevice != nullptr;
        }

        bool Device::ValidateIsInitialized() const
        {
            if (Validation::IsEnabled())
            {
                if (IsInitialized() == false)
                {
                    AZ_Error("Device", false, "Device is not initialized. This operation is invalid on an uninitialized device.");
                    return false;
                }
            }
            return true;
        }

        bool Device::ValidateIsInFrame() const
        {
            if (Validation::IsEnabled())
            {
                if (m_isInFrame == false)
                {
                    AZ_Error("Device", false, "Device is not currently processing a frame. This operation is only allowed within a BeginFrame / EndFrame scope.");
                    return false;
                }
            }
            return true;
        }

        bool Device::ValidateIsNotInFrame() const
        {
            if (Validation::IsEnabled())
            {
                if (m_isInFrame)
                {
                    AZ_Error("Device", false, "Device is currently processing a frame. This operation is only allowed outside of a BeginFrame / EndFrame scope.");
                    return false;
                }
            }
            return true;
        }

        ResultCode Device::Init(PhysicalDevice& physicalDevice)
        {
            if (Validation::IsEnabled())
            {
                if (IsInitialized())
                {
                    AZ_Error("Device", false, "Device is already initialized.");
                    return ResultCode::InvalidOperation;
                }
            }

            m_formatsCapabilities.fill(FormatCapabilities::None);
            m_nearestSupportedFormats.fill(Format::Unknown);
            
            m_physicalDevice = &physicalDevice;

            const ResultCode resultCode = InitInternal(physicalDevice);

            if (resultCode == ResultCode::Success)
            {
                FillFormatsCapabilitiesInternal(m_formatsCapabilities);

                // Fill supported format mapping for depth formats
                CalculateDepthStencilNearestSupportedFormats();

                // Any other supported format mapping should go here

                // Assume all formats that haven't been mapped yet are supported and map to themselves
                FillRemainingSupportedFormats();
            }
            else
            {
                m_physicalDevice = nullptr;
            }

            return resultCode;
        }
    
        ResultCode Device::PostInit(const DeviceDescriptor& descriptor)
        {
            if (Validation::IsEnabled())
            {
                if (!IsInitialized())
                {
                    AZ_Error("Device", false, "Device is not initialized.");
                    return ResultCode::InvalidOperation;
                }
            }

            m_descriptor = descriptor;
            const ResultCode resultCode = PostInitInternal(descriptor);

            if (resultCode != ResultCode::Success)
            {
                AZ_Error("Device", false, "Device is not initialized.");
                return ResultCode::InvalidOperation;
            }

            return resultCode;
        }

        void Device::Shutdown()
        {
            if (IsInitialized())
            {
                ShutdownInternal();
                m_physicalDevice = nullptr;
            }
        }

        ResultCode Device::BeginFrame()
        {
            AZ_TRACE_METHOD();

            if (ValidateIsInitialized() && ValidateIsNotInFrame())
            {
                m_isInFrame = true;
                BeginFrameInternal();
                return ResultCode::Success;
            }
            return ResultCode::InvalidOperation;
        }

        ResultCode Device::EndFrame()
        {
            if (ValidateIsInitialized() && ValidateIsInFrame())
            {
                EndFrameInternal();
                m_isInFrame = false;
                return ResultCode::Success;
            }
            return ResultCode::InvalidOperation;
        }

        ResultCode Device::WaitForIdle()
        {
            if (ValidateIsInitialized() && ValidateIsNotInFrame())
            {
                WaitForIdleInternal();
                return ResultCode::Success;
            }
            return ResultCode::InvalidOperation;
        }

        ResultCode Device::CompileMemoryStatistics(MemoryStatistics& memoryStatistics, MemoryStatisticsReportFlags reportFlags)
        {
            if (ValidateIsInitialized() && ValidateIsNotInFrame())
            {
                MemoryStatisticsBuilder builder;
                builder.Begin(memoryStatistics, reportFlags);
                CompileMemoryStatisticsInternal(builder);
                MemoryStatisticsEventBus::Event(this, &MemoryStatisticsEventBus::Events::ReportMemoryUsage, builder);
                builder.End();
                return ResultCode::Success;
            }
            return ResultCode::InvalidOperation;
        }

        ResultCode Device::UpdateCpuTimingStatistics(CpuTimingStatistics& cpuTimingStatistics) const
        {
            if (ValidateIsNotInFrame())
            {
                UpdateCpuTimingStatisticsInternal(cpuTimingStatistics);
                return ResultCode::Success;
            }
            return ResultCode::InvalidOperation;
        }

        const PhysicalDevice& Device::GetPhysicalDevice() const
        {
            return *m_physicalDevice;
        }

        const DeviceDescriptor& Device::GetDescriptor() const
        {
            return m_descriptor;
        }

        const DeviceFeatures& Device::GetFeatures() const
        {
            return m_features;
        }

        const DeviceLimits& Device::GetLimits() const
        {
            return m_limits;
        }

        const ResourcePoolDatabase& Device::GetResourcePoolDatabase() const
        {
            return m_resourcePoolDatabase;
        }

        ResourcePoolDatabase& Device::GetResourcePoolDatabase()
        {
            return m_resourcePoolDatabase;
        }

        FormatCapabilities Device::GetFormatCapabilities(Format format) const
        {
            return m_formatsCapabilities[static_cast<uint32_t>(format)];
        }        
      
        // Return the nearest supported format for this device.     
        Format Device::GetNearestSupportedFormat(Format requestedFormat, FormatCapabilities requestedCapabilities) const
        {
            Format nearestFormat = m_nearestSupportedFormats[aznumeric_caster(requestedFormat)];
            if (nearestFormat == Format::Unknown)
            {
                AZ_Assert(false, "The requested format [%s] has not been added to the mapping of device supported formats.", ToString(requestedFormat));
                return Format::Unknown;
            }            

            if (!RHI::CheckBitsAll(GetFormatCapabilities(nearestFormat), requestedCapabilities))
            {
                AZ_Assert(false, "The nearest found format [%s] does not support the requested format capabilities.", ToString(nearestFormat));
                return Format::Unknown;
            }

            return nearestFormat;
        }

        AZStd::vector<Format> Device::GetValidSwapChainImageFormats([[maybe_unused]] const WindowHandle& windowHandle) const
        {
            // [GFX TODO][ATOM-1125] Implement this method for every platform.  
            // After it, make this method pure virtual.
            return AZStd::vector<Format>{Format::R8G8B8A8_UNORM};
        }

        void Device::CalculateDepthStencilNearestSupportedFormats()
        {
            auto fillNearestFunc = [this](const AZStd::vector<Format>& formats)
            {
                for (int i = 0; i < formats.size(); ++i)
                {
                    // We search for formats that have at least the same bit depth.
                    for (int j = i; j < formats.size(); ++j)
                    {
                        if (RHI::CheckBitsAll(GetFormatCapabilities(formats[j]), FormatCapabilities::DepthStencil))
                        {
                            m_nearestSupportedFormats[aznumeric_caster(formats[i])] = formats[j];
                            break;
                        }
                    }

                    // Check if we found the nearest format. If not, then look for the closer format
                    // that may have less bit depth.
                    if (m_nearestSupportedFormats[aznumeric_caster(formats[i])] == Format::Unknown)
                    {
                        for (int j = i - 1; j >=0; --j)
                        {
                            if (RHI::CheckBitsAll(GetFormatCapabilities(formats[j]), FormatCapabilities::DepthStencil))
                            {
                                m_nearestSupportedFormats[aznumeric_caster(formats[i])] = formats[j];
                                break;
                            }
                        }
                    }
                }
            };

            const AZStd::vector<Format> depthFormats =
            {
                Format::D16_UNORM,
                Format::D32_FLOAT
            };

            const AZStd::vector<Format> depthStencilFormats =
            {
                Format::D16_UNORM_S8_UINT,
                Format::D24_UNORM_S8_UINT,
                Format::D32_FLOAT_S8X24_UINT
            };

            fillNearestFunc(depthFormats);
            fillNearestFunc(depthStencilFormats);
        }

        void Device::FillRemainingSupportedFormats()
        {
            size_t formatCount = static_cast<size_t>(Format::Count);

            for (size_t i = 0; i < formatCount; ++i)
            {
                if (m_nearestSupportedFormats[i] == Format::Unknown)
                {
                    m_nearestSupportedFormats[i] = static_cast<Format>(i);
                }
            }
        }
    }
}
