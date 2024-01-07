#include "pch.h"
#include "CGameObject.h"

#include "CComponent.h"
#include "CRenderComponent.h"

#include "CScript.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CGrabageCollector.h"

CGameObject::CGameObject()
    : m_arrCom{}
    , m_RenderCom(nullptr)
    , m_Parent(nullptr)
    , m_iLayerIdx(-1) // 어떠한 레벨(레이어) 소속되어있지 않다.
    , m_bDead(false)
{
}

CGameObject::~CGameObject()
{
    Delete_Array(m_arrCom);
    Delete_Vec(m_vecScript);
    Delete_Vec(m_vecChild);
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

    for (size_t i = 0; i < m_vecChild.size(); ++i)
    {
        m_vecChild[i]->begin();
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

    for (size_t i = 0; i < m_vecChild.size(); ++i)
    {
        m_vecChild[i]->tick();
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

    // 이번프레임에 렌더링할 레이어에 등록
    CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_iLayerIdx);
    pCurLayer->RegisterGameObject(this);

    // Dead 상태인 자식 오브젝트는 Grabage Collector에 보낸다.
    vector<CGameObject*>::iterator iter = m_vecChild.begin();
    for (; iter != m_vecChild.end();)
    {
        (*iter)->finaltick();

        if ((*iter)->m_bDead)
        {
            CGrabageCollector::GetInst()->Add(*iter);
            iter = m_vecChild.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void CGameObject::render()
{
    if (nullptr != m_RenderCom)
    {
        m_RenderCom->render();
    }

    for (size_t i = 0; i < m_vecChild.size(); ++i)
    {
        m_vecChild[i]->render();
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

void CGameObject::DisconnectWithParent()
{
    vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
    for (; iter != m_Parent->m_vecChild.end(); ++iter)
    {
        if (*iter == this)
        {
            m_Parent->m_vecChild.erase(iter);
            m_Parent = nullptr;
            return;
        }
    }

    // 부모가 없는 오브젝트에 DisconnectWithParent 함수를 호출 했거나
    // 부모는 자식을 가리키기지 않고 있는데, 자식은 부모를 가리키고 있는 경우
    assert(nullptr);
}

void CGameObject::DisconnectWithLayer()
{
    if (-1 == m_iLayerIdx)
        return;

    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    CLayer* pCurLayer = pCurLevel->GetLayer(m_iLayerIdx);
    pCurLayer->DetachGameObject(this);
}

void CGameObject::AddChild(CGameObject* _Child)
{
    if (_Child->m_Parent)
    {
        // 이전 부모 오브젝트랑 연결 해제
        _Child->DisconnectWithParent();
    }

    // 부모 자식 연결
    _Child->m_Parent = this;
    m_vecChild.push_back(_Child);
}

void CGameObject::Destroy()
{
    GamePlayStatic::DestroyGameObject(this);
}

void CGameObject::SetLayer(const wstring& _strLayerName)
{
    SetLayer(CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(_strLayerName)->GetLayerIdx());
}
