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

#if !defined(Q_MOC_RUN)
#include <QMainWindow>

#include <AzCore/Math/Guid.h>
#include <AzCore/Math/Uuid.h>
#endif

// This is the shell class to interface between Qt and the Sandbox.  All Sandbox implementation is retained in an inherited class.
class ComponentSliceFavoritesWindow
    : public QMainWindow
{
    Q_OBJECT

public:
    explicit ComponentSliceFavoritesWindow(QWidget* parent = 0);
    ~ComponentSliceFavoritesWindow();

    void Init();

    // you are required to implement this to satisfy the unregister/registerclass requirements on "AzToolsFramework::RegisterViewPane"
    // make sure you pick a unique GUID
    static const GUID& GetClassID()
    {
        // {DAC19E32-A83F-FF41-8A9C-1AA161FA67FF}
        static const GUID guid =
        {
            0xDAC19E32, 0xA83F, 0xFF41,{ 0x8a, 0x9c, 0x1a, 0xa1, 0x61, 0xfa, 0x67, 0xff }
        };
        return guid;
    }

};
