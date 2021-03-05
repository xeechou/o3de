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

// Description : This file implements the container for the assotiaon of
//               enumeration name to enumeration values


#include "EditorDefs.h"

#include "UIEnumerations.h"

//////////////////////////////////////////////////////////////////////////
CUIEnumerations& CUIEnumerations::GetUIEnumerationsInstance()
{
    static CUIEnumerations  oGeneralProxy;
    return oGeneralProxy;
}

//////////////////////////////////////////////////////////////////////////
CUIEnumerations::TDValuesContainer& CUIEnumerations::GetStandardNameContainer()
{
    static TDValuesContainer    cValuesContainer;
    static bool                             boInit(false);

    if (!boInit)
    {
        boInit = true;

        XmlNodeRef                          oRootNode;
        XmlNodeRef                          oEnumaration;
        XmlNodeRef                          oEnumerationItem;

        int                                         nNumberOfEnumarations(0);
        int                                         nCurrentEnumaration(0);

        int                                         nNumberOfEnumerationItems(0);
        int                                         nCurrentEnumarationItem(0);

        oRootNode = GetISystem()->GetXmlUtils()->LoadXmlFromFile("Editor\\PropertyEnumerations.xml");
        nNumberOfEnumarations = oRootNode ? oRootNode->getChildCount() : 0;

        for (nCurrentEnumaration = 0; nCurrentEnumaration < nNumberOfEnumarations; ++nCurrentEnumaration)
        {
            TDValues    cValues;
            oEnumaration = oRootNode->getChild(nCurrentEnumaration);

            nNumberOfEnumerationItems = oEnumaration->getChildCount();
            for (nCurrentEnumarationItem = 0; nCurrentEnumarationItem < nNumberOfEnumerationItems; ++nCurrentEnumarationItem)
            {
                oEnumerationItem = oEnumaration->getChild(nCurrentEnumarationItem);

                const char* szKey(NULL);
                const char* szValue(NULL);
                oEnumerationItem->getAttributeByIndex(0, &szKey, &szValue);

                cValues.push_back(szValue);
            }

            const char* szKey(NULL);
            const char* szValue(NULL);
            oEnumaration->getAttributeByIndex(0, &szKey, &szValue);

            cValuesContainer.insert(TDValuesContainer::value_type(szValue, cValues));
        }
    }

    return cValuesContainer;
}
//////////////////////////////////////////////////////////////////////////
