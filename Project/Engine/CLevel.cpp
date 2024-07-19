#include "pch.h"
#include "CLevel.h"

#include "CTimeMgr.h"
#include "CRenderMgr.h"
#include "CPhysics2DMgr.h"
#include "CPhysicsMgr.h"

#include "CLayer.h"
#include "CGameObject.h"

CLevel::CLevel()
    : m_arrLayer{}
    , m_State(LEVEL_STATE::STOP)
    , m_PrevState(m_State)
    , m_StepFrames(0)
{
    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i] = new CLayer;
        m_arrLayer[i]->m_iLayerIdx = i;
    }
}

CLevel::CLevel(const CLevel& origin)
    : CEntity(origin)
    , m_arrLayer{}
    , m_State(LEVEL_STATE::STOP)
    , m_PrevState(m_State)
    , m_StepFrames(origin.m_StepFrames)
{
    for (size_t i = 0; i < LAYER_MAX; i++)
    {
        m_arrLayer[i] = origin.m_arrLayer[i]->Clone();
    }
}

CLevel::~CLevel()
{
    Delete_Array(m_arrLayer);
}

void CLevel::begin()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i]->begin();
    }
}

void CLevel::tick()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i]->tick();
    }
}

void CLevel::finaltick()
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i]->finaltick();
    }

    // Step
    if (0 < m_StepFrames)
    {
        m_StepFrames--;

        if (0 == m_StepFrames)
            CTimeMgr::GetInst()->LockDeltaTime(true);
    }
}

void CLevel::AddObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
    if (nullptr == _Object || _LayerIdx < 0 || _LayerIdx >= LAYER_MAX)
        return;

    m_arrLayer[_LayerIdx]->AddObject(_Object, _bChildMove);
}

void CLevel::AddObject(CGameObject* _Object, const wstring& _strLayerName, bool _bChildMove)
{
    if (nullptr == _Object)
        return;

    CLayer* pLayer = GetLayer(_strLayerName);
    if (nullptr == pLayer)
        return;

    pLayer->AddObject(_Object, _bChildMove);
}

CLayer* CLevel::GetLayer(const wstring& _strLayerName) const
{
    for (int i = 0; i < LAYER_MAX; ++i)
    {
        if (_strLayerName == m_arrLayer[i]->GetName())
        {
            return m_arrLayer[i];
        }
    }
    return nullptr;
}

CGameObject* CLevel::FindObjectByName(const wstring& _strName)
{
    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        if (nullptr == m_arrLayer[i])
            continue;

        const vector<CGameObject*>& vecParent = m_arrLayer[i]->GetParentObjects();

        for (size_t j = 0; j < vecParent.size(); ++j)
        {
            list<CGameObject*> queue;
            queue.push_back(vecParent[j]);

            // 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
            while (!queue.empty())
            {
                CGameObject* pObject = queue.front();
                queue.pop_front();

                if (nullptr == pObject)
                    continue;

                const vector<CGameObject*>& vecChild = pObject->GetChildObject();
                for (size_t k = 0; k < vecChild.size(); ++k)
                {
                    queue.push_back(vecChild[k]);
                }

                if (_strName == pObject->GetName())
                {
                    return pObject;
                }
            }
        }
    }

    return nullptr;
}

CGameObject* CLevel::FindObjectByName(const wstring& _strName, int _LayerIdx)
{
    if (_LayerIdx < 0 || _LayerIdx >= LAYER_MAX)
        return nullptr;

    const vector<CGameObject*>& vecParent = m_arrLayer[_LayerIdx]->GetParentObjects();

    for (size_t j = 0; j < vecParent.size(); ++j)
    {
        list<CGameObject*> queue;
        queue.push_back(vecParent[j]);

        // 레이어에 입력되는 오브젝트 포함, 그 밑에 달린 자식들까지 모두 확인
        while (!queue.empty())
        {
            CGameObject* pObject = queue.front();
            queue.pop_front();

            if (nullptr == pObject)
                continue;

            const vector<CGameObject*>& vecChild = pObject->GetChildObject();
            for (size_t k = 0; k < vecChild.size(); ++k)
            {
                queue.push_back(vecChild[k]);
            }

            if (_strName == pObject->GetName())
            {
                return pObject;
            }
        }
    }

    return nullptr;
}

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
    // 레벨 상태 변경
    m_PrevState = m_State;
    m_State = _NextState;

    if (m_PrevState == _NextState)
    {
        if (LEVEL_STATE::PLAY == m_PrevState)
        {
            CTimeMgr::GetInst()->LockDeltaTime(false);
            CRenderMgr::GetInst()->ActiveEditorMode(false);
        }
        else if (LEVEL_STATE::SIMULATE == m_PrevState)
        {
            CTimeMgr::GetInst()->LockDeltaTime(false);
            CRenderMgr::GetInst()->ActiveEditorMode(true);
        }
        else if (LEVEL_STATE::PAUSE == m_PrevState)
        {
            CTimeMgr::GetInst()->LockDeltaTime(true);
            CRenderMgr::GetInst()->ActiveEditorMode(true);
        }
        else if (LEVEL_STATE::STOP == m_PrevState)
        {
            CTimeMgr::GetInst()->LockDeltaTime(true);
            CRenderMgr::GetInst()->ActiveEditorMode(true);
            CPhysics2DMgr::GetInst()->OnPhysics2DStop();
            CPhysicsMgr::GetInst()->OnPhysicsStop();
        }
    }
    else
    {
        switch (m_PrevState)
        {
        case LEVEL_STATE::PLAY: {
            if (LEVEL_STATE::PAUSE == _NextState)
            {
                CRenderMgr::GetInst()->ActiveEditorMode(true);
                CTimeMgr::GetInst()->LockDeltaTime(true);
            }
            else if (LEVEL_STATE::STOP == _NextState)
            {
                CTimeMgr::GetInst()->LockDeltaTime(true);
                CRenderMgr::GetInst()->ActiveEditorMode(true);
                CPhysics2DMgr::GetInst()->OnPhysics2DStop();
                CPhysicsMgr::GetInst()->OnPhysicsStop();
            }
        }
        break;
        case LEVEL_STATE::SIMULATE: {
            CTimeMgr::GetInst()->LockDeltaTime(true);

            // if (LEVEL_STATE::PAUSE == _NextState)
            //{
            // }
            if (LEVEL_STATE::STOP == _NextState)
            {
                CRenderMgr::GetInst()->ActiveEditorMode(true);
                CPhysics2DMgr::GetInst()->OnPhysics2DStop();
                CPhysicsMgr::GetInst()->OnPhysicsStop();
            }
        }
        break;
        case LEVEL_STATE::PAUSE: {
            if (LEVEL_STATE::PLAY == _NextState)
            {
                CRenderMgr::GetInst()->ActiveEditorMode(false);
                CTimeMgr::GetInst()->LockDeltaTime(false);
            }
            else if (LEVEL_STATE::SIMULATE == _NextState)
            {
                CTimeMgr::GetInst()->LockDeltaTime(false);
            }
            else if (LEVEL_STATE::STOP == _NextState)
            {
                CTimeMgr::GetInst()->LockDeltaTime(true);
                CRenderMgr::GetInst()->ActiveEditorMode(true);
                CPhysics2DMgr::GetInst()->OnPhysics2DStop();
                CPhysicsMgr::GetInst()->OnPhysicsStop();
            }
        }
        break;
        case LEVEL_STATE::STOP: {
            CTimeMgr::GetInst()->LockDeltaTime(false);
            CPhysics2DMgr::GetInst()->OnPhysics2DStart();
            CPhysicsMgr::GetInst()->OnPhysicsStart();
            CRenderMgr::GetInst()->render_StaticShadowDepth();
            begin();

            if (LEVEL_STATE::PLAY == _NextState)
            {
                CRenderMgr::GetInst()->ActiveEditorMode(false);
            }
            else if (LEVEL_STATE::SIMULATE == _NextState)
            {
                CRenderMgr::GetInst()->ActiveEditorMode(true);
            }
        }
        break;
        }
    }
}

void CLevel::Step(int _Frames)
{
    m_StepFrames = _Frames;
    CTimeMgr::GetInst()->LockDeltaTime(false);
}

int CLevel::FindLayerIndexByName(const wstring& _strName)
{
    int Idx = -1;
    for (int i = 0; i < LAYER_MAX; i++)
    {
        if (_strName == GetLayer(i)->GetName())
        {
            Idx = i;
            break;
        }
    }

    return Idx;
}

wstring CLevel::FindLayerNameByIndex(int _Index)
{
    if (_Index < 0 || _Index >= LAYER_MAX)
        return wstring();

    return GetLayer(_Index)->GetName();
}

void CLevel::clear()
{
    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i]->m_vecObjects.clear();
    }
}