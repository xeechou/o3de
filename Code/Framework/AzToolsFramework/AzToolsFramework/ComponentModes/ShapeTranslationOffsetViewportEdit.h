/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Component/ComponentBus.h>
#include <AzToolsFramework/Manipulators/TranslationManipulators.h>

namespace AzToolsFramework
{
    //! Wraps translation manipulators for editing shape translation offsets.
    class ShapeTranslationOffsetViewportEdit
    {
    public:
        ShapeTranslationOffsetViewportEdit() = default;

        void Setup(const AZ::EntityComponentIdPair& entityComponentIdPair);
        void Teardown();
        void UpdateManipulators();

    private:
        AZ::EntityComponentIdPair m_entityComponentIdPair;
        AZStd::shared_ptr<TranslationManipulators> m_translationManipulators; //!< Manipulators for editing shape offset.
    };
} // namespace AzToolsFramework
