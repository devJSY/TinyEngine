#include "pch.h"
#include "CLevelChangeScript.h"

CLevelChangeScript::CLevelChangeScript()
    : CScript(LEVELCHANGESCRIPT)
{
}

CLevelChangeScript::~CLevelChangeScript()
{
}

void CLevelChangeScript::tick()
{
    //GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(L"HallofEternity_0.tLevel"), LEVEL_STATE::PLAY);
}
