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
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#include "EditorDefs.h"

#include "GizmoManager.h"

// Editor
#include "Gizmo.h"


//////////////////////////////////////////////////////////////////////////
void CGizmoManager::Display(DisplayContext& dc)
{
    FUNCTION_PROFILER(GetIEditor()->GetSystem(), PROFILE_EDITOR);

    AABB bbox;
    std::vector<CGizmo*> todelete;
    for (Gizmos::iterator it = m_gizmos.begin(); it != m_gizmos.end(); ++it)
    {
        CGizmo* gizmo = *it;
        if (gizmo->GetFlags() & EGIZMO_HIDDEN)
        {
            continue;
        }

        gizmo->GetWorldBounds(bbox);
        if (dc.IsVisible(bbox))
        {
            gizmo->Display(dc);
        }

        if (gizmo->IsDelete())
        {
            todelete.push_back(gizmo);
        }
    }

    // Delete gizmos that needs deletion.
    for (int i = 0; i < todelete.size(); i++)
    {
        RemoveGizmo(todelete[i]);
    }
}

//////////////////////////////////////////////////////////////////////////
void CGizmoManager::AddGizmo(CGizmo* gizmo)
{
    m_gizmos.insert(gizmo);
}

//////////////////////////////////////////////////////////////////////////
void CGizmoManager::RemoveGizmo(CGizmo* gizmo)
{
    m_gizmos.erase(gizmo);
}

//////////////////////////////////////////////////////////////////////////
int CGizmoManager::GetGizmoCount() const
{
    return (int)m_gizmos.size();
}

//////////////////////////////////////////////////////////////////////////
CGizmo* CGizmoManager::GetGizmoByIndex(int nIndex) const
{
    int nCount = 0;
    Gizmos::iterator ii = m_gizmos.begin();
    for (; ii != m_gizmos.end(); ++ii)
    {
        if ((nCount++) == nIndex)
        {
            return *ii;
        }
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
bool CGizmoManager::HitTest(HitContext& hc)
{
    float mindist = FLT_MAX;

    HitContext ghc = hc;
    bool bGizmoHit = false;

    AABB bbox;
    for (Gizmos::iterator it = m_gizmos.begin(); it != m_gizmos.end(); ++it)
    {
        CGizmo* gizmo = *it;

        if (gizmo->GetFlags() & EGIZMO_SELECTABLE)
        {
            if (gizmo->HitTest(ghc))
            {
                bGizmoHit = true;
                if (ghc.dist < mindist)
                {
                    mindist = ghc.dist;
                    hc = ghc;
                }
            }
        }
    }
    return bGizmoHit;
}

//////////////////////////////////////////////////////////////////////////
void CGizmoManager::DeleteAllTransformManipulators()
{
    std::vector<CGizmo*> todelete;
    for (Gizmos::iterator it = m_gizmos.begin(); it != m_gizmos.end(); ++it)
    {
        CGizmo* gizmo = *it;
        if (gizmo->GetFlags() & EGIZMO_TRANSFORM_MANIPULATOR)
        {
            todelete.push_back(gizmo);
        }
    }

    // Delete gizmos that needs deletion.
    for (int i = 0; i < todelete.size(); i++)
    {
        RemoveGizmo(todelete[i]);
    }
}
