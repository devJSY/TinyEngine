#include "pch.h"
#include "CLevelFlowMgr.h"

#include <Engine/CLevelSaveLoad.h>
#include <Engine/CLevelMgr.h>

CLevelFlowMgr::CLevelFlowMgr(UINT _Type)
    : CScript(_Type)
    , m_CurLevel(nullptr)
    , m_NextLevel(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_NextLevelPath, "Next Level Name");
}

CLevelFlowMgr::CLevelFlowMgr(const CLevelFlowMgr& _Origin)
    : CScript(_Origin)
    , m_CurLevel(nullptr)
    , m_NextLevel(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_NextLevelPath, "Next Level Name");
}

CLevelFlowMgr::~CLevelFlowMgr()
{
}

void CLevelFlowMgr::LevelInit()
{
    // BGM 재생
    // UI (Fade In)
}

void CLevelFlowMgr::LevelExit()
{
    // BGM 종료
    // UI (Fade Out)

    // Level Change
    CLevelMgr::GetInst()->ChangeLevel(m_NextLevel,LEVEL_STATE::PLAY);
}

void CLevelFlowMgr::LevelRestart()
{
    // BGM 종료
    // UI (Fade Out)

    // Level Restart
    CLevelMgr::GetInst()->ChangeLevel(m_CurLevel->Clone(), LEVEL_STATE::PLAY);
}

void CLevelFlowMgr::begin()
{
    m_CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    m_NextLevel = CLevelSaveLoad::LoadLevel(ToWstring(m_NextLevelPath));
}

void CLevelFlowMgr::tick()
{

}

UINT CLevelFlowMgr::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += SaveWStringToFile(ToWstring(m_NextLevelPath), _File);

    return MemoryByte;
}

UINT CLevelFlowMgr::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring NextLvName;
    MemoryByte += LoadWStringFromFile(NextLvName, _File);

    m_NextLevelPath = ToString(NextLvName);

    return MemoryByte;
}
