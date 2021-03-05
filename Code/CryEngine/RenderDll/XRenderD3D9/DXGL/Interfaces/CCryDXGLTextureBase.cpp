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

// Description : Definition of the DXGL common base class for textures

#include "RenderDll_precompiled.h"
#include "CCryDXGLTextureBase.hpp"
#include "../Implementation/GLResource.hpp"


CCryDXGLTextureBase::CCryDXGLTextureBase(D3D11_RESOURCE_DIMENSION eDimension, NCryOpenGL::STexture* pGLTexture, CCryDXGLDevice* pDevice)
    : CCryDXGLResource(eDimension, pGLTexture, pDevice)
{
}

CCryDXGLTextureBase::~CCryDXGLTextureBase()
{
}

NCryOpenGL::STexture* CCryDXGLTextureBase::GetGLTexture()
{
    return static_cast<NCryOpenGL::STexture*>(m_spGLResource.get());
}
