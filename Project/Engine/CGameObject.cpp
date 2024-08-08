#include "pch.h"
#include "CGameObject.h"

#include "CLevelMgr.h"
#include "CPhysics2DMgr.h"
#include <Scripts\\CScriptMgr.h>

#include "CComponent.h"
#include "CRenderComponent.h"

#include "CScript.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CGrabageCollector.h"

CGameObject::CGameObject()
    : m_arrCom{}
    , m_RenderCom(nullptr)
    , m_Parent(nullptr)
    , m_iLayerIdx(-1) // 어떠한 레벨(레이어) 소속되어있지 않다.
    , m_bActive(true)
    , m_bDead(false)
    , m_BoneSocket(nullptr)
{
}

CGameObject::CGameObject(const CGameObject& origin)
    : CEntity(origin)
    , m_arrCom{}
    , m_RenderCom(nullptr)
    , m_Parent(nullptr)
    , m_iLayerIdx(origin.m_iLayerIdx)
    , m_bActive(origin.m_bActive)
    , m_bDead(false)
    , m_BoneSocket(origin.m_BoneSocket)
{
    for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
    {
        if (nullptr == origin.m_arrCom[i])
            continue;

        CComponent* pComp = origin.m_arrCom[i]->Clone();
        if (nullptr == pComp)
        {
            LOG(Error, "%s Clone Failed!!", COMPONENT_TYPE_STRING[i]);
        }
        else
        {
            AddComponent(pComp);
        }
    }

    for (size_t i = 0; i < origin.m_vecScript.size(); ++i)
    {
        CScript* pScript = origin.m_vecScript[i]->Clone();
        if (nullptr == pScript)
        {
            LOG(Error, "%s Clone Failed!!", ToString(CScriptMgr::GetScriptName(origin.m_vecScript[i])).c_str());
        }
        else
        {
            AddComponent(pScript);
        }
    }

    for (size_t i = 0; i < origin.m_vecChild.size(); ++i)
    {
        AddChild(origin.m_vecChild[i]->Clone());
    }
}

CGameObject::~CGameObject()
{
    Delete_Array(m_arrCom);
    Delete_Vec(m_vecScript);
    Delete_Vec(m_vecChild);
}

void CGameObject::begin()
{
    // 비활성화 상태이거나 레이어에 소속되어있는 오브젝트가 아닌경우
    if (!m_bActive || -1 == m_iLayerIdx)
        return;

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

void CGameObject::tick()
{
    // 비활성화 상태이거나 레이어에 소속되어있는 오브젝트가 아닌경우
    if (!m_bActive || -1 == m_iLayerIdx)
        return;

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

void CGameObject::finaltick()
{
    // 비활성화 상태이거나 레이어에 소속되어있는 오브젝트가 아닌경우
    if (!m_bActive || -1 == m_iLayerIdx)
        return;

    for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
    {
        if (nullptr == m_arrCom[i])
            continue;

        m_arrCom[i]->finaltick();
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
    if (nullptr == m_RenderCom)
        return;

    m_RenderCom->render();
}

void CGameObject::render(Ptr<CMaterial> _mtrl)
{
    if (nullptr == m_RenderCom)
        return;

    m_RenderCom->render(_mtrl);
}

void CGameObject::AddComponent(CComponent* _Component)
{
    COMPONENT_TYPE type = _Component->GetComponentType();

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

void CGameObject::RemoveComponent(COMPONENT_TYPE _Type)
{
    // 해당 타입의 컴포넌트가 존재하지 않은 경우
    if (nullptr == m_arrCom[(UINT)_Type])
        return;

    // Transform 은 삭제 불가
    if (COMPONENT_TYPE::TRANSFORM == _Type)
        return;

    // Render Component
    if (COMPONENT_TYPE::MESHRENDER == _Type || COMPONENT_TYPE::TILEMAP == _Type || COMPONENT_TYPE::PARTICLESYSTEM == _Type ||
        COMPONENT_TYPE::SKYBOX == _Type || COMPONENT_TYPE::DECAL == _Type || COMPONENT_TYPE::LANDSCAPE == _Type ||
        COMPONENT_TYPE::TEXTRENDER == _Type)
    {
        m_RenderCom = nullptr;
    }

    delete m_arrCom[(UINT)_Type];
    m_arrCom[(UINT)_Type] = nullptr;
}

void CGameObject::RemoveScript(CScript* _script)
{
    std::vector<CScript*>::iterator iter = m_vecScript.begin();

    for (; iter != m_vecScript.end(); iter++)
    {
        if ((*iter) == _script)
        {
            m_vecScript.erase(iter);
            delete _script;
            _script = nullptr;
            break;
        }
    }
}

void CGameObject::DisconnectWithParent()
{
    if (nullptr == m_Parent)
        return;

    bool bSuccess = false;
    vector<CGameObject*>::iterator iter = m_Parent->m_vecChild.begin();
    for (; iter != m_Parent->m_vecChild.end(); ++iter)
    {
        if (*iter == this)
        {
            m_Parent->m_vecChild.erase(iter);
            m_Parent = nullptr;
            m_BoneSocket = nullptr;
            bSuccess = true;
            break;
        }
    }

    // 부모는 자식을 가리키기지 않고 있는데, 자식은 부모를 가리키고 있는 경우
    if (!bSuccess)
    {
        assert(nullptr);
    }
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
    else
    {
        // 자식으로 들어오는 오브젝트가 최상위 부모타입이면,
        // 소속 레이어의 Parent 오브젝트 목록에서 제거한다.
        int iLayerIdx = _Child->m_iLayerIdx;
        _Child->DisconnectWithLayer();
        _Child->m_iLayerIdx = iLayerIdx;
    }

    // 자식이 지정된 레이어가 존재하지 않을경우 부모 레이어로 설정
    if (-1 == _Child->m_iLayerIdx)
        _Child->m_iLayerIdx = m_iLayerIdx;

    // 부모 자식 연결
    _Child->m_Parent = this;
    m_vecChild.push_back(_Child);
}

bool CGameObject::IsActive() const
{
    bool bActive = m_bActive;

    // 부모가 비활성화 상태인경우 자식도 비활성화 처리
    if (bActive)
    {
        CGameObject* pParent = GetParent();

        while (pParent)
        {
            bActive = pParent->IsActive();

            // 비활성화 감지
            if (!bActive)
                break;

            pParent = pParent->GetParent();
        }
    }

    return bActive;
}

void CGameObject::SetActive(bool _bActive)
{
    if (m_bActive == _bActive)
        return;

    m_bActive = _bActive;

    // 본인 포함 자식오브젝트들 Physics 리셋
    list<CGameObject*> queue;
    queue.push_back(this);

    while (!queue.empty())
    {
        CGameObject* pObject = queue.front();
        queue.pop_front();

        const vector<CGameObject*>& vecChildObj = pObject->GetChildObject();

        for (size_t i = 0; i < vecChildObj.size(); ++i)
        {
            queue.push_back(vecChildObj[i]);
        }

        GamePlayStatic::Physics2D_Event(pObject, Physics2D_EVENT_TYPE::RESPAWN);
        GamePlayStatic::Physics_Event(pObject, Physics_EVENT_TYPE::RESPAWN);
    }
}

bool CGameObject::IsAncestor(CGameObject* _Other)
{
    CGameObject* pParent = m_Parent;

    while (pParent)
    {
        if (pParent == _Other)
            return true;

        pParent = pParent->m_Parent;
    }

    return false;
}

CGameObject* CGameObject::GetChildObject(const wstring& _Name) const
{
    for (const auto& iter : m_vecChild)
    {
        if (iter->GetName() == _Name)
        {
            return iter;
        }
    }

    return nullptr;
}

void CGameObject::DetachBoneSockets()
{
    for (CGameObject* pChild : m_vecChild)
    {
        if (nullptr != pChild->GetBoneSocket())
        {
            pChild->SetBoneSocket(nullptr);
        }
    }
}
