#include "pch.h"
#include "CExitButton.h"
#include <Engine/CLevelSaveLoad.h>

CExitButton::CExitButton()
    : CButtonScript(EXITBUTTON)
{
}

CExitButton::CExitButton(const CExitButton& origin)
    : CButtonScript(origin)
{
}

CExitButton::~CExitButton()
{
}

void CExitButton::Func()
{
    GamePlayStatic::ChangeLevelState(CLevelMgr::GetInst()->GetCurrentLevel(), LEVEL_STATE::STOP);
}