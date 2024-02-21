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

#include "CTestLevel.h"
#include "CPBRLevel.h"

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
    // Level
    m_CurLevel = new CTestLevel;
    m_CurLevel->SetName(L"Test Level");

    // PBR Level
    // m_CurLevel = new CPBRLevel;
    // m_CurLevel->SetName(L"PBR Level");

    m_CurLevel->ChangeState(LEVEL_STATE::PLAY);
}

void CLevelMgr::tick()
{
    if (nullptr == m_CurLevel)
        return;

    // 이전 프레임에 등록된 오브젝트들 clear
    m_CurLevel->clear();

    m_CurLevel->tick();
    m_CurLevel->finaltick();
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
    if (nullptr != m_CurLevel)
    {
        delete m_CurLevel;
        m_CurLevel = nullptr;
    }

    m_CurLevel = _NextLevel;
}

void CLevelMgr::ChangeLevelState(LEVEL_STATE _State)
{
    tTask task = {};

    task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
    task.Param_1 = (DWORD_PTR)m_CurLevel;
    task.Param_2 = (DWORD_PTR)_State;

    CTaskMgr::GetInst()->AddTask(task);
}
