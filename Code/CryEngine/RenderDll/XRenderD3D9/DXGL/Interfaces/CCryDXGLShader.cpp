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

// Description : Definition of the DXGL wrapper for D3D11 shader interfaces


#include "RenderDll_precompiled.h"
#include "CCryDXGLShader.hpp"
#include "../Implementation/GLShader.hpp"

CCryDXGLShader::CCryDXGLShader(NCryOpenGL::SShader* pGLShader, CCryDXGLDevice* pDevice)
    : CCryDXGLDeviceChild(pDevice)
    , m_spGLShader(pGLShader)
{
}

CCryDXGLShader::~CCryDXGLShader()
{
}

NCryOpenGL::SShader* CCryDXGLShader::GetGLShader()
{
    return m_spGLShader.get();
}