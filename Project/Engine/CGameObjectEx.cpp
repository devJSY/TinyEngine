#include "pch.h"
#include "CGameObjectEx.h"

#include "CComponent.h"
#include "CScript.h"

void CGameObjectEx::begin()
{
    for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
    {
        if (nullptr != m_arrCom[i])
        {
            m_arrCom[i]->begin();
        }
    }

    for (size_t i = 0; i < m_vecScript.size(); ++i)
    {
        m_vecScript[i]->begin();
    }

    for (size_t i = 0; i < m_vecChild.size(); ++i)
    {
        m_vecChild[i]->begin();
    }
}

void CGameObjectEx::tick()
{
    for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
    {
        if (nullptr == m_arrCom[i])
            continue;

        m_arrCom[i]->tick();
    }

    for (size_t i = 0; i < m_vecScript.size(); ++i)
    {
        m_vecScript[i]->tick();
    }

    for (size_t i = 0; i < m_vecChild.size(); ++i)
    {
        m_vecChild[i]->tick();
    }
}

void CGameObjectEx::finaltick()
{
    for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
    {
        if (nullptr == m_arrCom[i])
            continue;

        m_arrCom[i]->finaltick();
    }

    vector<CGameObject*>::const_iterator iter = GetChildObject().begin();
    for (; iter != GetChildObject().end(); ++iter)
    {
        (*iter)->finaltick();
    }
}
