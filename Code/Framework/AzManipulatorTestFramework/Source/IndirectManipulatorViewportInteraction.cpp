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

#include <AzManipulatorTestFramework/IndirectManipulatorViewportInteraction.h>
#include <AzManipulatorTestFramework/ViewportInteraction.h>
#include <AzToolsFramework/Manipulators/ManipulatorManager.h>

namespace AzManipulatorTestFramework
{
    using MouseInteraction = AzToolsFramework::ViewportInteraction::MouseInteraction;
    using MouseInteractionEvent = AzToolsFramework::ViewportInteraction::MouseInteractionEvent;

    //! Implementation of the manipulator interface using bus calls to access to the manipulator manager.
    class IndirectCallManipulatorManager
        : public ManipulatorManagerInterface
    {
    public:
        IndirectCallManipulatorManager(ViewportInteractionInterface& viewportInteraction);
        // ManipulatorManagerInterface ...
        void ConsumeMouseInteractionEvent(const MouseInteractionEvent& event) override;
        AzToolsFramework::ManipulatorManagerId GetId() const override;
        bool ManipulatorBeingInteracted() const override;

    private:
        // trigger the updating of manipulator bounds.
        void DrawManipulators();
        ViewportInteractionInterface& m_viewportInteraction;
    };

    IndirectCallManipulatorManager::IndirectCallManipulatorManager(ViewportInteractionInterface& viewportInteraction)
        : m_viewportInteraction(viewportInteraction)
    {
    }

    void IndirectCallManipulatorManager::ConsumeMouseInteractionEvent(const MouseInteractionEvent& event)
    {
        DrawManipulators();
        AzToolsFramework::EditorInteractionSystemViewportSelectionRequestBus::Event(
            AzToolsFramework::GetEntityContextId(),
            &AzToolsFramework::ViewportInteraction::InternalMouseViewportRequests::InternalHandleAllMouseInteractions,
            event);
        DrawManipulators();
    }

    void IndirectCallManipulatorManager::DrawManipulators()
    {
        AzFramework::ViewportDebugDisplayEventBus::Event(
            AzToolsFramework::GetEntityContextId(), &AzFramework::ViewportDebugDisplayEvents::DisplayViewport,
            AzFramework::ViewportInfo{ m_viewportInteraction.GetViewportId() }, m_viewportInteraction.GetDebugDisplay());
    }

    AzToolsFramework::ManipulatorManagerId IndirectCallManipulatorManager::GetId() const
    {
        return AzToolsFramework::g_mainManipulatorManagerId;
    }

    bool IndirectCallManipulatorManager::ManipulatorBeingInteracted() const
    {
        bool manipulatorInteracting;
        AzToolsFramework::ManipulatorManagerRequestBus::EventResult(
            manipulatorInteracting, AzToolsFramework::g_mainManipulatorManagerId,
            &AzToolsFramework::ManipulatorManagerRequestBus::Events::Interacting);

        return manipulatorInteracting;
    }

    IndirectCallManipulatorViewportInteraction::IndirectCallManipulatorViewportInteraction()
        : m_viewportInteraction(AZStd::make_unique<ViewportInteraction>())
        , m_manipulatorManager(AZStd::make_unique<IndirectCallManipulatorManager>(*m_viewportInteraction))
    {
    }

    IndirectCallManipulatorViewportInteraction::~IndirectCallManipulatorViewportInteraction() = default;

    const ViewportInteractionInterface& IndirectCallManipulatorViewportInteraction::GetViewportInteraction() const
    {
        return *m_viewportInteraction;
    }

    const ManipulatorManagerInterface& IndirectCallManipulatorViewportInteraction::GetManipulatorManager() const
    {
        return *m_manipulatorManager;
    }
} // namespace AzManipulatorTestFramework
