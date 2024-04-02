#include "pch.h"
#include "CPlayButtonScript.h"
#include "CLevelChangeScript.h"

CPlayButtonScript::CPlayButtonScript()
    : CButtonScript(PLAYBUTTONSCRIPT)
{
}

CPlayButtonScript::~CPlayButtonScript()
{
}

void CPlayButtonScript::tick()
{
    CButtonScript::tick();

    if (m_bClicked)
    {
        CGameManagerScript::GetInset()->ChangeLevel("Hall of Eternity Floor 0");
    }
}
