#include "pch.h"
#include "CEnemyFailedAttack.h"

CEnemyFailedAttack::CEnemyFailedAttack()
{
}

CEnemyFailedAttack::~CEnemyFailedAttack()
{
}

void CEnemyFailedAttack::Enter()
{
    GetOwner()->GetComponent<CAnimator>()->Play(MONSTERANIM(L"Brake"),false);
}

void CEnemyFailedAttack::tick()
{
    CAnimator* _anim = GetOwner()->GetComponent<CAnimator>();
    if (_anim->IsFinish() && _anim->FindClipIndex(MONSTERANIM(L"Brake")) == _anim->GetCurClipIdx())
    {
        GetOwner()->GetComponent<CAnimator>()->Play(MONSTERANIM(L"LookAround"),false);
    }

    if (_anim->IsFinish() && _anim->FindClipIndex(MONSTERANIM(L"LookAround")) == _anim->GetCurClipIdx())
    {
        ChangeState(L"IDLE");
    }
}

void CEnemyFailedAttack::Exit()
{
}