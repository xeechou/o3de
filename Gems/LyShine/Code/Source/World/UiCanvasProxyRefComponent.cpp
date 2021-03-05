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
#include "LyShine_precompiled.h"
#include "UiCanvasProxyRefComponent.h"
#include <AzCore/Serialization/SerializeContext.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <LyShine/Bus/UiCanvasBus.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC MEMBER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
UiCanvasProxyRefComponent::UiCanvasProxyRefComponent()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////
AZ::EntityId UiCanvasProxyRefComponent::GetCanvas()
{
    AZ::EntityId uiCanvasEntityId;

    if (m_canvasAssetRefEntityId.IsValid())
    {
        EBUS_EVENT_ID_RESULT(uiCanvasEntityId, m_canvasAssetRefEntityId, UiCanvasRefBus, GetCanvas);
    }

    return uiCanvasEntityId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiCanvasProxyRefComponent::SetCanvasRefEntity(AZ::EntityId canvasAssetRefEntity)
{
    m_canvasAssetRefEntityId = canvasAssetRefEntity;

    AZ::EntityId uiCanvasEntityId = GetCanvas();

    EBUS_EVENT_ID(GetEntityId(), UiCanvasRefNotificationBus, OnCanvasRefChanged, GetEntityId(), uiCanvasEntityId);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiCanvasProxyRefComponent::OnCanvasRefChanged([[maybe_unused]] AZ::EntityId uiCanvasRefEntity, AZ::EntityId uiCanvasEntity)
{
    EBUS_EVENT_ID(GetEntityId(), UiCanvasRefNotificationBus, OnCanvasRefChanged, GetEntityId(), uiCanvasEntity);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PUBLIC STATIC MEMBER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiCanvasProxyRefComponent::Reflect(AZ::ReflectContext* context)
{
    AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(context);

    if (serializeContext)
    {
        serializeContext->Class<UiCanvasProxyRefComponent, AZ::Component>()
            ->Version(1)
            ->Field("CanvasAssetRefEntity", &UiCanvasProxyRefComponent::m_canvasAssetRefEntityId);

        AZ::EditContext* editContext = serializeContext->GetEditContext();
        if (editContext)
        {
            auto editInfo = editContext->Class<UiCanvasProxyRefComponent>("UI Canvas Proxy Ref", "The UI Canvas Proxy Ref component allows you to associate an entity with another entity that is managing a UI Canvas");

            editInfo->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                ->Attribute(AZ::Edit::Attributes::Category, "UI")
                ->Attribute(AZ::Edit::Attributes::Icon, "Editor/Icons/Components/UiCanvasProxyRef.svg")
                ->Attribute(AZ::Edit::Attributes::ViewportIcon, "Editor/Icons/Components/Viewport/UiCanvasRef.png")
                ->Attribute(AZ::Edit::Attributes::HelpPageURL, "https://docs.aws.amazon.com/lumberyard/latest/userguide/component-ui-canvas-proxy-ref.html")
                ->Attribute(AZ::Edit::Attributes::AppearsInAddComponentMenu, AZ_CRC("Game", 0x232b318c));

            editInfo->DataElement(0, &UiCanvasProxyRefComponent::m_canvasAssetRefEntityId,
                "Canvas Asset Ref entity", "The entity that holds the UI Canvas Asset Ref component.");
        }
    }

    AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context);
    if (behaviorContext)
    {
        behaviorContext->EBus<UiCanvasProxyRefBus>("UiCanvasProxyRefBus")
            ->Event("SetCanvasRefEntity", &UiCanvasProxyRefBus::Events::SetCanvasRefEntity);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// PROTECTED MEMBER FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiCanvasProxyRefComponent::Activate()
{
    UiCanvasRefBus::Handler::BusConnect(GetEntityId());
    UiCanvasProxyRefBus::Handler::BusConnect(GetEntityId());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void UiCanvasProxyRefComponent::Deactivate()
{
    UiCanvasProxyRefBus::Handler::BusDisconnect();
    UiCanvasRefBus::Handler::BusDisconnect();
}
