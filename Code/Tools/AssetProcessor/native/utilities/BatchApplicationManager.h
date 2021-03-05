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
#include "native/utilities/ApplicationManagerBase.h"
#endif

namespace AssetProcessor
{
    extern const char ExcludeMetaDataFiles[];
}

class BatchApplicationManager
    : public ApplicationManagerBase
    , public AssetProcessor::MessageInfoBus::Handler
{
    Q_OBJECT
public:
    explicit BatchApplicationManager(int* argc, char*** argv, QObject* parent = 0);
    virtual ~BatchApplicationManager();

    void Destroy() override;
    bool Activate() override;

    ////////////////////////////////////////////////////
    ///MessageInfoBus::Listener interface///////////////
    void OnErrorMessage(const char* error) override;
    ///////////////////////////////////////////////////

    bool InitApplicationServer() override;

private:
    void Reflect() override;
    const char* GetLogBaseName() override;
    RegistryCheckInstructions PopupRegistryProblemsMessage(QString warningText) override;
    void InitSourceControl() override;

    void MakeActivationConnections() override;

    bool GetShouldExitOnIdle() const override { return true; }

    void TryScanProductDependencies() override;
    void TryHandleFileRelocation() override;
};
