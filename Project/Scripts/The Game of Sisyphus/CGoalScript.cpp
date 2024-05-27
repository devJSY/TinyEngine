#include "pch.h"
#include "CGoalScript.h"
#include <Engine\\CLevelSaveLoad.h>

CGoalScript::CGoalScript()
    : CScript(GOALSCRIPT)
{
}

CGoalScript::~CGoalScript()
{
}

void CGoalScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (L"Ball" != _OtherCollider->GetOwner()->GetName())
        return;

    // ¿ÁΩ√¿€
    GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(L"The Game of Sisyphus.tLevel"), LEVEL_STATE::PLAY);
}
