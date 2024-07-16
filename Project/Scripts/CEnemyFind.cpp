#include "pch.h"
#include "CEnemyFind.h"

CEnemyFind::CEnemyFind()
{
}

CEnemyFind::~CEnemyFind()
{
}

void CEnemyFind::Enter()
{
    GetOwner()->GetComponent<CAnimator>()->Play(MONSTERANIM(L"Find"),false);
}

void CEnemyFind::tick()
{
    if (GetOwner()->GetComponent<CAnimator>()->IsFinish())
    {
        ChangeState(L"ATTACK");
    }
}

void CEnemyFind::Exit()
{
}