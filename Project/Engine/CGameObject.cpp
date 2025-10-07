#include "pch.h"
#include "CGameObject.h"

#include "CLevelMgr.h"
#include "CPhysics2DMgr.h"
#include "CTaskMgr.h"
#include <Scripts/CScriptMgr.h>

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
    , m_iLayerIdx(-1) // ��� ����(���̾�) �ҼӵǾ����� �ʴ�.
    , m_bActive(true)
    , m_bDead(false)
    , m_bBeginCalled(false)
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
    , m_bBeginCalled(false)
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
    // ��Ȱ��ȭ ����
    // ���̾ �ҼӵǾ��ִ� ������Ʈ�� �ƴ� ���
    // begin �� �̹� ���� �� ���
    if (!m_bActive || -1 == m_iLayerIdx || m_bBeginCalled)
        return;

    m_bBeginCalled = true;

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
    // ��Ȱ��ȭ �����̰ų� ���̾ �ҼӵǾ��ִ� ������Ʈ�� �ƴѰ��
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
    // ��Ȱ��ȭ �����̰ų� ���̾ �ҼӵǾ��ִ� ������Ʈ�� �ƴѰ��
    if (!m_bActive || -1 == m_iLayerIdx)
        return;

    for (UINT i = 0; i < UINT(COMPONENT_TYPE::END); ++i)
    {
        if (nullptr == m_arrCom[i])
            continue;

        m_arrCom[i]->finaltick();
    }

    // �̹������ӿ� �������� ���̾ ���
    CLayer* pCurLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_iLayerIdx);
    pCurLayer->RegisterGameObject(this);

    // Dead ������ �ڽ� ������Ʈ�� Grabage Collector�� ������.
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
        // Script Ÿ�� Component �� ������ Script Ŭ������ �ƴ� ���
        if (nullptr == dynamic_cast<CScript*>(_Component))
        {
            assert(nullptr);
            return;
        }

        m_vecScript.push_back((CScript*)_Component);
        _Component->m_Owner = this;
    }
    else
    {
        // �̹� �ش� Ÿ���� ������Ʈ�� �����ϰ� �ִ� ���
        if (nullptr != m_arrCom[(UINT)type])
        {
            assert(nullptr);
            return;
        }

        m_arrCom[(UINT)type] = _Component;
        _Component->m_Owner = this;

        CRenderComponent* pRenderCom = dynamic_cast<CRenderComponent*>(_Component);
        if (nullptr != pRenderCom)
        {
            // �̹� �� ���� �̻��� RenderComponent �� �����ϰ� �ִ� ���
            if (nullptr != m_RenderCom)
            {
                assert(nullptr);
                return;
            }

            m_RenderCom = pRenderCom;
        }
    }
}

void CGameObject::RemoveComponent(COMPONENT_TYPE _Type)
{
    // �ش� Ÿ���� ������Ʈ�� �������� ���� ���
    if (nullptr == m_arrCom[(UINT)_Type])
        return;

    // Transform �� ���� �Ұ�
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

    // �θ�� �ڽ��� ����Ű���� �ʰ� �ִµ�, �ڽ��� �θ� ����Ű�� �ִ� ���
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
        // ���� �θ� ������Ʈ�� ���� ����
        _Child->DisconnectWithParent();
    }
    else
    {
        // �ڽ����� ������ ������Ʈ�� �ֻ��� �θ�Ÿ���̸�,
        // �Ҽ� ���̾��� Parent ������Ʈ ��Ͽ��� �����Ѵ�.
        int iLayerIdx = _Child->m_iLayerIdx;
        _Child->DisconnectWithLayer();
        _Child->m_iLayerIdx = iLayerIdx;
    }

    // �ڽ��� ������ ���̾ �������� ������� �θ� ���̾�� ����
    if (-1 == _Child->m_iLayerIdx)
        _Child->m_iLayerIdx = m_iLayerIdx;

    // �θ� �ڽ� ����
    _Child->m_Parent = this;
    m_vecChild.push_back(_Child);
}

bool CGameObject::IsActive() const
{
    bool bActive = m_bActive;

    // �θ� ��Ȱ��ȭ �����ΰ�� �ڽĵ� ��Ȱ��ȭ ó��
    if (bActive)
    {
        CGameObject* pParent = GetParent();

        while (pParent)
        {
            bActive = pParent->IsActive();

            // ��Ȱ��ȭ ����
            if (!bActive)
                break;

            pParent = pParent->GetParent();
        }
    }

    return bActive;
}

void CGameObject::SetActive(bool _bActive)
{
    tTask task = {};
    task.Type = TASK_TYPE::ACTIVE_OBJECT;
    task.Param_1 = (DWORD_PTR)this;
    task.Param_2 = (DWORD_PTR)_bActive;
    CTaskMgr::GetInst()->AddTask(task);
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
