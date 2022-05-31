/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <AzCore/Component/Component.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/std/containers/deque.h>
#include <AzCore/std/smart_ptr/shared_ptr.h>
#include <AzCore/std/parallel/shared_mutex.h>
#include <AzCore/std/parallel/atomic.h>

#include <Atom/Feature/Utils/FrameCaptureBus.h>

#include <Atom/RPI.Public/Pass/AttachmentReadback.h>

namespace AZ
{
    namespace Render
    {
        //! System component to handle FrameCaptureRequestBus.
        class FrameCaptureSystemComponent final
            : public AZ::Component
            , public FrameCaptureRequestBus::Handler
            , public AZ::SystemTickBus::Handler
        {
        public:
            AZ_COMPONENT(FrameCaptureSystemComponent, "{53931220-19E7-4DE4-AF29-C4BB16E251D1}");

            static void Reflect(AZ::ReflectContext* context);

            void Activate() override;
            void Deactivate() override;

            // FrameCaptureRequestBus overrides ...
            bool CanCapture() const override;
            uint32_t CaptureScreenshot(const AZStd::string& filePath) override;
            uint32_t CaptureScreenshotForWindow(const AZStd::string& filePath, AzFramework::NativeWindowHandle windowHandle) override;
            uint32_t CaptureScreenshotWithPreview(const AZStd::string& outputFilePath) override;
            uint32_t CapturePassAttachment(const AZStd::vector<AZStd::string>& passHierarchy, const AZStd::string& slotName, const AZStd::string& outputFilePath,
                RPI::PassAttachmentReadbackOption option) override;
            uint32_t CapturePassAttachmentWithCallback(const AZStd::vector<AZStd::string>& passHierarchy, const AZStd::string& slotName
                , RPI::AttachmentReadback::CallbackFunction callback, RPI::PassAttachmentReadbackOption option) override;

        private:
            struct CaptureState
            {
                CaptureState(uint32_t captureId);
                CaptureState(CaptureState&& rhs);
                void Reset();
                AZStd::shared_ptr<AZ::RPI::AttachmentReadback> m_readback;
                AZStd::string m_outputFilePath;

                AZStd::atomic<FrameCaptureResult> m_result = FrameCaptureResult::None; // use atomic as this is written and read on different threads.
                AZStd::string m_latestCaptureInfo;
            };
            class CaptureHandle
            {
            public:
                CaptureHandle(FrameCaptureSystemComponent* frameCaptureSystemComponent, uint32_t captureStateIndex);
                CaptureHandle() = delete;

                static CaptureHandle Null();

                // match AZStd::mutex function names so scoped_lock works
                void lock();
                void unlock();

                CaptureState* GetCaptureState();
                uint32_t GetCaptureStateIndex() {return m_captureStateIndex;}
                bool IsValid() {return m_captureStateIndex != InvalidCaptureHandle;};
                bool IsNull() {return m_captureStateIndex == InvalidCaptureHandle;};
            private:
                static constexpr uint32_t InvalidCaptureHandle = FrameCaptureRequests::s_InvalidFrameCaptureId;

                FrameCaptureSystemComponent* const m_frameCaptureSystemComponent;
                const uint32_t m_captureStateIndex;
            };
            friend CaptureHandle;

            CaptureHandle  InternalCapturePassAttachment(const AZStd::vector<AZStd::string>& passHierarchy, 
                                                         const AZStd::string& slotName, 
                                                         const AZStd::string& outputFilePath,
                                                         RPI::PassAttachmentReadbackOption option);

            void CaptureAttachmentCallback(const AZ::RPI::AttachmentReadback::ReadbackResult& readbackResult);

            CaptureHandle InitCapture();

            // SystemTickBus overrides ...
            void OnSystemTick() override;

            AZStd::string ResolvePath(const AZStd::string& filePath);

            AZStd::shared_mutex m_handleLock; // Use a shared_mutex so we can protect against the allCaptures vector being resized and moving the CaptureStates in memory.
            AZStd::deque<CaptureHandle> m_idleCaptures; // fifo for idle captures
            AZStd::deque<CaptureHandle> m_inProgressCaptures; // uses a deque so we can maintain order, order created == order OnCaptureFinished sent
            AZStd::vector<CaptureState> m_allCaptures;
        };
    }
}
