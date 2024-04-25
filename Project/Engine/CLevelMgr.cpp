#include "pch.h"
#include "CEngine.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CDevice.h"
#include "CEditorMgr.h"
#include "CEditor.h"
#include "CLayer.h"
#include "CRenderMgr.h"
#include "CTaskMgr.h"

#include "CLevelSaveLoad.h"
#include "components.h"

CLevelMgr::CLevelMgr()
    : m_CurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
    if (nullptr != m_CurLevel)
    {
        delete m_CurLevel;
        m_CurLevel = nullptr;
    }
}

void CLevelMgr::init()
{
    m_CurLevel = new CLevel;
    m_CurLevel->ChangeState(LEVEL_STATE::STOP);
}

void CLevelMgr::tick()
{
    if (nullptr == m_CurLevel)
        return;

    // 이전 프레임에 등록된 오브젝트들 clear
    m_CurLevel->clear();

    if (m_CurLevel->GetState() == LEVEL_STATE::PLAY || m_CurLevel->GetState() == LEVEL_STATE::SIMULATE)
    {
        m_CurLevel->tick();
    }

    m_CurLevel->finaltick();
}

CLevel* CLevelMgr::CreateNewLevel()
{
    CLevel* NewLevel = new CLevel;
    NewLevel->SetName(L"New Level");

    for (int i = 0; i < LAYER_MAX; i++)
    {
        wstring Name = L"Layer ";
        Name += std::to_wstring(i);
        NewLevel->GetLayer(i)->SetName(Name);
    }

    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerMaskAll();
    pCamObj->Camera()->LayerMask(NewLevel, L"UI", false);
    pCamObj->Camera()->SetHDRI(false);

    NewLevel->AddObject(pCamObj, 0);

    // UI 만 렌더링
    pCamObj = new CGameObject;
    pCamObj->SetName(L"UI Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(1);
    pCamObj->Camera()->LayerMask(NewLevel, L"UI", true);

    NewLevel->AddObject(pCamObj, 0);

    return NewLevel;
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel, LEVEL_STATE _StartState)
{
    if (m_CurLevel == _NextLevel)
        return;

    if (nullptr != m_CurLevel)
        delete m_CurLevel;

    m_CurLevel = _NextLevel;

    if (nullptr != m_CurLevel)
        m_CurLevel->ChangeState(_StartState);
}
