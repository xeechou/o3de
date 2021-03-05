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

#include "PhysX_precompiled.h"
#include <Tests/EditorTestUtilities.h>
#include <EditorRigidBodyComponent.h>
#include <PhysXCharacters/Components/EditorCharacterControllerComponent.h>

namespace PhysXEditorTests
{
    TEST_F(PhysXEditorFixture, EditorCharacterControllerComponent_CharacterControllerPlusRigidBodyComponents_EntityIsInvalid)
    {
        EntityPtr entity = CreateInactiveEditorEntity("ShapeColliderComponentEditorEntity");
        entity->CreateComponent<PhysX::EditorCharacterControllerComponent>();
        entity->CreateComponent<PhysX::EditorRigidBodyComponent>();

        // the entity should be in an invalid state because the character controller is incompatible with the rigid body
        AZ::Entity::DependencySortOutcome sortOutcome = entity->EvaluateDependenciesGetDetails();
        EXPECT_FALSE(sortOutcome.IsSuccess());
        EXPECT_TRUE(sortOutcome.GetError().m_code == AZ::Entity::DependencySortResult::HasIncompatibleServices);
    }
} // namespace PhysXEditorTests
