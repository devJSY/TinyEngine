#include "pch.h"
#include "CLevelChangeButtonScript.h"

#include <Engine/CLevelSaveLoad.h>

CLevelChangeButtonScript::CLevelChangeButtonScript()
    : CButtonScript(LEVELCHANGEBUTTONSCRIPT)
    , m_LevelName{}
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_LevelName, "Level Name");
}

CLevelChangeButtonScript::~CLevelChangeButtonScript()
{
}

void CLevelChangeButtonScript::Func()
{
    GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(ToWstring(m_LevelName)), LEVEL_STATE::PLAY);
}

UINT CLevelChangeButtonScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CButtonScript::SaveToLevelFile(_File);
    MemoryByte += SaveWStringToFile(ToWstring(m_LevelName), _File);

    return MemoryByte;
}

UINT CLevelChangeButtonScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CButtonScript::LoadFromLevelFile(_File);

    wstring _SaveLevel = {};
    MemoryByte += LoadWStringFromFile(_SaveLevel, _File);
    m_LevelName = ToString(_SaveLevel);

    return MemoryByte;
}
