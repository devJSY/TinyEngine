#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"

#include "CScript.h"

CGameObject::CGameObject()
    : m_arrCom{}
    , m_RenderCom(nullptr)
{
}

CGameObject::~CGameObject()
{
    Delete_Array(m_arrCom);
    Delete_Vec(m_vecScript);
}

void CGameObject::begin()
{
    for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
    {
        if (nullptr != m_arrCom[i])
        {
            m_arrCom[i]->begin();
        }
    }
}

void CGameObject::tick()
{
    for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
    {
        if (nullptr != m_arrCom[i])
        {
            m_arrCom[i]->tick();
        }
    }

    for (size_t i = 0; i < m_vecScript.size(); ++i)
    {
        m_vecScript[i]->tick();
    }
}

void CGameObject::finaltick()
{
    for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
    {
        if (nullptr != m_arrCom[i])
        {
            m_arrCom[i]->finaltick();
        }
    }
}

void CGameObject::render()
{
    if (nullptr != m_RenderCom)
    {
        m_RenderCom->render();
    }
}

void CGameObject::AddComponent(CComponent* _Component)
{
    COMPONENT_TYPE type = _Component->GetType();

    if (type == COMPONENT_TYPE::SCRIPT)
    {
        // Script 타입 Component 가 실제로 Script 클래스가 아닌 경우
        assert(dynamic_cast<CScript*>(_Component));

        m_vecScript.push_back((CScript*)_Component);
        _Component->m_Owner = this;
    }
    else
    {
        // 이미 해당 타입의 컴포넌트를 보유하고 있는 경우
        assert(!m_arrCom[(UINT)type]);

        m_arrCom[(UINT)type] = _Component;
        _Component->m_Owner = this;

        CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);
        if (nullptr != pRenderCom)
        {
            // 이미 한 종류 이상의 RenderComponent 를 보유하고 있는 경우
            assert(!m_RenderCom);

            m_RenderCom = pRenderCom;
        }
    }
}
