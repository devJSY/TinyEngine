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
        GamePlayStatic::Play2DSound(L"sound\\FX\\Click.wav", 1, 0.3f);
        GamePlayStatic::Play2DSound(L"sound\\FX\\PC_Book_UI_Transition_01.wav", 1, 0.3f);
        CGameManagerScript::GetInset()->ChangeLevel("Hall of Eternity Floor 0");
    }
}
