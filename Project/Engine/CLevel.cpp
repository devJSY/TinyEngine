#include "pch.h"
#include "CLevel.h"

#include "CTimeMgr.h"
#include "CRenderMgr.h"
#include "CPhysics2DMgr.h"

#include "CLayer.h"

CLevel::CLevel()
    : m_arrLayer{}
    , m_State(LEVEL_STATE::STOP)
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
}

void CLevel::AddObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove)
{
    m_arrLayer[_LayerIdx]->AddObject(_Object, _bChildMove);
}

void CLevel::AddObject(CGameObject* _Object, const wstring& _strLayerName, bool _bChildMove)
{
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

void CLevel::ChangeState(LEVEL_STATE _NextState)
{
    if (m_State == _NextState)
    {
        if (LEVEL_STATE::PLAY == m_State)
        {
            CTimeMgr::GetInst()->LockDeltaTime(false);
            CRenderMgr::GetInst()->ActiveEditorMode(false);
        }
        else if (LEVEL_STATE::PAUSE == m_State)
        {
            CTimeMgr::GetInst()->LockDeltaTime(true);
            CRenderMgr::GetInst()->ActiveEditorMode(true);
        }
        else if (LEVEL_STATE::STOP == m_State)
        {
            CTimeMgr::GetInst()->LockDeltaTime(true);
            CRenderMgr::GetInst()->ActiveEditorMode(true);
            CPhysics2DMgr::GetInst()->OnPhysics2DStop();
        }
    }
    // 정지 -> 플레이
    else if ((LEVEL_STATE::STOP == m_State || LEVEL_STATE::PAUSE == m_State) && LEVEL_STATE::PLAY == _NextState)
    {
        CTimeMgr::GetInst()->LockDeltaTime(false);

        // 레벨 카메라 모드
        CRenderMgr::GetInst()->ActiveEditorMode(false);

        if (LEVEL_STATE::STOP == m_State)
        {
            begin();
            CPhysics2DMgr::GetInst()->OnPhysics2DStart();
        }
    }

    // 플레이 -> 정지 or 일시정지
    else if (LEVEL_STATE::PLAY == m_State && (LEVEL_STATE::STOP == _NextState || LEVEL_STATE::PAUSE == _NextState))
    {
        CTimeMgr::GetInst()->LockDeltaTime(true);

        // 에디터 카메라 모드
        CRenderMgr::GetInst()->ActiveEditorMode(true);

        if (LEVEL_STATE::STOP == _NextState)
        {
            CPhysics2DMgr::GetInst()->OnPhysics2DStop();
        }
    }

    // 일시정지 → 정지
    else if (LEVEL_STATE::PAUSE == m_State && LEVEL_STATE::STOP == _NextState)
    {
        CPhysics2DMgr::GetInst()->OnPhysics2DStop();
    }

    // 레벨 스테이트 변경
    m_State = _NextState;
}

void CLevel::clear()
{
    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        m_arrLayer[i]->m_vecObjects.clear();
    }
}