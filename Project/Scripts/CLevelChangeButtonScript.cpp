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

void CLevelChangeButtonScript::SaveToLevelFile(FILE* _File)
{
    CButtonScript::SaveToLevelFile(_File);

    SaveWStringToFile(ToWstring(m_LevelName), _File);
}

void CLevelChangeButtonScript::LoadFromLevelFile(FILE* _File)
{
    CButtonScript::LoadFromLevelFile(_File);

    wstring _SaveLevel = {};
    LoadWStringFromFile(_SaveLevel, _File);
    m_LevelName = ToString(_SaveLevel);
}
