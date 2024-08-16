#include "pch.h"
#include "CLevelChangeButtonScript.h"

#include <Engine/CLevelSaveLoad.h>

#include "CLevelFlowMgr.h"

CLevelChangeButtonScript::CLevelChangeButtonScript()
    : CButtonScript(LEVELCHANGEBUTTONSCRIPT)
    , m_pManager(nullptr)
{
}

CLevelChangeButtonScript::CLevelChangeButtonScript(const CLevelChangeButtonScript& Origin)
    : CButtonScript(Origin)
    , m_pManager(nullptr)
{
}

CLevelChangeButtonScript::~CLevelChangeButtonScript()
{
}

void CLevelChangeButtonScript::begin()
{
    CButtonScript::begin();

    m_pManager = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");
}

void CLevelChangeButtonScript::Func()
{
    if (nullptr != m_pManager)
    {
        m_pManager->GetScript<CLevelFlowMgr>()->LevelEnd();
    }
}

UINT CLevelChangeButtonScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = CButtonScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CLevelChangeButtonScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = CButtonScript::LoadFromLevelFile(_File);

    return MemoryByte;
}
