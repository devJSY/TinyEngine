#include "pch.h"
#include "CGameObjectEx.h"

#include "CComponent.h"

void CGameObjectEx::finaltick()
{
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr != GetComponent((COMPONENT_TYPE)i))
        {
            GetComponent((COMPONENT_TYPE)i)->finaltick();
        }
    }

    vector<CGameObject*>::const_iterator iter = GetChildObject().begin();
    for (; iter != GetChildObject().end(); ++iter)
    {
        (*iter)->finaltick();
    }
}
