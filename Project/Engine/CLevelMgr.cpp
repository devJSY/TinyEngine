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

    m_CurLevel->tick();
    m_CurLevel->finaltick();
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
