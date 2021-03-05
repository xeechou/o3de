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


// CryCommon
#include <CryCommon/Maestro/Types/AnimParamType.h>
// Editor
#include "TrackViewKeyPropertiesDlg.h"

//////////////////////////////////////////////////////////////////////////
class CCaptureKeyUIControls
    : public CTrackViewKeyUIControls
{
public:
    CSmartVariableArray mv_table;
    CSmartVariable<float> mv_duration;
    CSmartVariable<float> mv_timeStep;
    CSmartVariableEnum<QString> mv_format;
    CSmartVariable<QString> mv_prefix;
    CSmartVariable<QString> mv_folder;
    CSmartVariableEnum<int> mv_captureBufferType;
    CSmartVariable<bool> mv_once;

    virtual void OnCreateVars()
    {
        mv_duration.GetVar()->SetLimits(0, 100000.0f);
        mv_timeStep.GetVar()->SetLimits(0.001f, 1.0f);

        // mv_format enumerations must match ICaptureKey::CaptureFileFormat enum
        mv_format.SetEnumList(NULL);
        mv_format->AddEnumItem("jpg", "jpg");
        mv_format->AddEnumItem("tga", "tga");
        mv_format->AddEnumItem("tif", "tif");

        mv_captureBufferType.SetEnumList(NULL);
        mv_captureBufferType->AddEnumItem("Color", ICaptureKey::Color);
        mv_captureBufferType->AddEnumItem("Color+Alpha", ICaptureKey::ColorWithAlpha);

        AddVariable(mv_table, "Key Properties");
        AddVariable(mv_table, mv_duration, "Duration");
        AddVariable(mv_table, mv_timeStep, "Time Step");
        AddVariable(mv_table, mv_format, "Output Format");
        AddVariable(mv_table, mv_prefix, "Output Prefix");
        AddVariable(mv_table, mv_folder, "Output Folder");
        AddVariable(mv_table, mv_captureBufferType, "Buffer(s) to capture");
        AddVariable(mv_table, mv_once, "Just one frame?");
    }
    bool SupportTrackType(const CAnimParamType& paramType, [[maybe_unused]] EAnimCurveType trackType, [[maybe_unused]] AnimValueType valueType) const
    {
        return paramType == AnimParamType::Capture;
    }
    virtual bool OnKeySelectionChange(CTrackViewKeyBundle& selectedKeys);
    virtual void OnUIChange(IVariable* pVar, CTrackViewKeyBundle& selectedKeys);

    virtual unsigned int GetPriority() const { return 1; }

    static const GUID& GetClassID()
    {
        // {543197BF-5E43-4abc-8F07-B84078846E4C}
        static const GUID guid =
        {
            0x543197bf, 0x5e43, 0x4abc, { 0x8f, 0x7, 0xb8, 0x40, 0x78, 0x84, 0x6e, 0x4c }
        };
        return guid;
    }
};

//////////////////////////////////////////////////////////////////////////
bool CCaptureKeyUIControls::OnKeySelectionChange(CTrackViewKeyBundle& selectedKeys)
{
    if (!selectedKeys.AreAllKeysOfSameType())
    {
        return false;
    }

    bool bAssigned = false;
    if (selectedKeys.GetKeyCount() == 1)
    {
        const CTrackViewKeyHandle& keyHandle = selectedKeys.GetKey(0);

        CAnimParamType paramType = keyHandle.GetTrack()->GetParameterType();
        if (paramType == AnimParamType::Capture)
        {
            ICaptureKey captureKey;
            keyHandle.GetKey(&captureKey);

            mv_duration = captureKey.duration;
            mv_timeStep = captureKey.timeStep;
            mv_format = captureKey.GetFormat();
            mv_prefix = captureKey.prefix.c_str();
            mv_folder = captureKey.folder.c_str();
            mv_captureBufferType = captureKey.captureBufferIndex;
            mv_once = captureKey.once;

            bAssigned = true;
        }
    }
    return bAssigned;
}

// Called when UI variable changes.
void CCaptureKeyUIControls::OnUIChange(IVariable* pVar, CTrackViewKeyBundle& selectedKeys)
{
    CTrackViewSequence* sequence = GetIEditor()->GetAnimation()->GetSequence();

    if (!sequence || !selectedKeys.AreAllKeysOfSameType())
    {
        return;
    }

    for (unsigned int keyIndex = 0; keyIndex < selectedKeys.GetKeyCount(); ++keyIndex)
    {
        CTrackViewKeyHandle keyHandle = selectedKeys.GetKey(keyIndex);

        CAnimParamType paramType = keyHandle.GetTrack()->GetParameterType();
        if (paramType == AnimParamType::Capture)
        {
            ICaptureKey captureKey;
            keyHandle.GetKey(&captureKey);

            SyncValue(mv_duration, captureKey.duration, false, pVar);
            SyncValue(mv_timeStep, captureKey.timeStep, false, pVar);
            if (pVar == mv_format.GetVar())
            {
                if (QString::compare(mv_format, "jpg") == 0)
                {
                    captureKey.FormatJPG();
                }
                else if (QString::compare(mv_format, "bmp") == 0)
                {
                    captureKey.FormatBMP();
                }
                else if (QString::compare(mv_format, "hdr") == 0)
                {
                    captureKey.FormatHDR();
                }
                else
                {
                    captureKey.FormatTGA();
                }
            }
            if (pVar == mv_folder.GetVar())
            {
                QString sFolder = mv_folder;
                captureKey.folder = sFolder.toUtf8().data();
            }
            if (pVar == mv_prefix.GetVar())
            {
                QString sPrefix = mv_prefix;
                captureKey.prefix = sPrefix.toUtf8().data();
            }
            if (pVar == mv_captureBufferType.GetVar())
            {
                captureKey.captureBufferIndex = static_cast<ICaptureKey::CaptureBufferType>(static_cast<int>(mv_captureBufferType));
            }
            SyncValue(mv_once, captureKey.once, false, pVar);

            bool isDuringUndo = false;
            AzToolsFramework::ToolsApplicationRequests::Bus::BroadcastResult(isDuringUndo, &AzToolsFramework::ToolsApplicationRequests::Bus::Events::IsDuringUndoRedo);

            if (isDuringUndo)
            {
                keyHandle.SetKey(&captureKey);
            }
            else
            {
                AzToolsFramework::ScopedUndoBatch undoBatch("Set Key Value");
                keyHandle.SetKey(&captureKey);
                undoBatch.MarkEntityDirty(sequence->GetSequenceComponentEntityId());
            }
        }
    }
}

REGISTER_QT_CLASS_DESC(CCaptureKeyUIControls, "TrackView.KeyUI.Capture", "TrackViewKeyUI");
