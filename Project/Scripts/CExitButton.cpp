#include "pch.h"
#include "CExitButton.h"
#include <Engine/CLevelSaveLoad.h>

CExitButton::CExitButton()
    : CButtonScript(EXITBUTTON)
{
}

CExitButton::~CExitButton()
{
}

void CExitButton::Func()
{
    GamePlayStatic::ChangeLevelState(CLevelMgr::GetInst()->GetCurrentLevel(), LEVEL_STATE::STOP);
}