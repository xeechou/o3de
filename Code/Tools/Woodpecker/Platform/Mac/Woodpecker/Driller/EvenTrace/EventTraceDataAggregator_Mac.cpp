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

#include <QtCore/QString>
#include <QtCore/QProcess>
#include <QtCore/QDir>

namespace Driller
{
    namespace Platform
    {
        void LaunchExplorerSelect(const QString& filePath)
        {
            QProcess::startDetached("/usr/bin/osascript", {"-e",
                QStringLiteral("tell application \"Finder\" to reveal POSIX file \"%1\"").arg(QDir::toNativeSeparators(filePath))});
            QProcess::startDetached("/usr/bin/osascript", {"-e",
                QStringLiteral("tell application \"Finder\" to activate")});
        }
    }
}
