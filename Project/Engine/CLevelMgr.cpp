#include "pch.h"
#include "CEngine.h"
#include "CLevel.h"
#include "CLevelMgr.h"
#include "CTaskMgr.h"
#include "CDevice.h"
#include "CEditorMgr.h"
#include "CEditor.h"
#include "CLayer.h"

#include "CLevelSaveLoad.h"

#include "CTestLevel2.h"
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

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
    if (nullptr != m_CurLevel)
    {
        delete m_CurLevel;
        m_CurLevel = nullptr;
    }

    m_CurLevel = _NextLevel;
}

void CLevelMgr::init()
{
    // Level 2
    //m_CurLevel = new CTestLevel2;
    //m_CurLevel->SetName(L"Test Level 2");

    // PBR Level
    m_CurLevel = new CPBRLevel;
    m_CurLevel->SetName(L"PBR Level");
    
    //m_CurLevel = CLevelSaveLoad::LoadLevel(L"PBRLevel.tLevel");

    m_CurLevel->begin();
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
