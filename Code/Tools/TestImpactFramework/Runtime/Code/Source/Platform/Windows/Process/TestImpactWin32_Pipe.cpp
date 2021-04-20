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

#include "TestImpactWin32_Pipe.h"

#include <Process/TestImpactProcessException.h>

#include <AzCore/std/containers/vector.h>
#include <AzCore/std/optional.h>

namespace TestImpact
{
    Pipe::Pipe(SECURITY_ATTRIBUTES& sa, HANDLE& stdChannel)
    {
        if (!CreatePipe(&m_parent, &m_child, &sa, 0))
        {
            throw ProcessException("Couldn't create pipe");
        }

        SetHandleInformation(m_parent, HANDLE_FLAG_INHERIT, 0);
        stdChannel = m_child;
    }

    void Pipe::ReleaseChild()
    {
        m_child.Close();
    }

    void Pipe::EmptyPipe()
    {
        DWORD bytesAvailable = 0;
        while (PeekNamedPipe(m_parent, NULL, 0, NULL, &bytesAvailable, NULL) && bytesAvailable > 0)
        {
            // Grow the buffer by the number of bytes available in the pipe and append the new data
            DWORD bytesRead;
            const size_t currentSize = m_buffer.size();
            m_buffer.resize(m_buffer.size() + bytesAvailable);
            if (!ReadFile(m_parent, m_buffer.data() + currentSize, bytesAvailable, &bytesRead, NULL) || bytesRead == 0)
            {
                throw ProcessException("Couldn't read child output from pipe");
            }
        }
    }

    AZStd::string Pipe::GetContentsAndClearInternalBuffer()
    {
        EmptyPipe();
        AZStd::string contents;

        if (m_buffer.size() > 0)
        {
            contents = AZStd::string(m_buffer.begin(), m_buffer.end());
            m_buffer.clear();
        }

        return contents;
    }
} // namespace TestImpact
