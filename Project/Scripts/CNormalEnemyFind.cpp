#include "pch.h"
#include "CNormalEnemyFind.h"

CNormalEnemyFind::CNormalEnemyFind()
{
}

CNormalEnemyFind::~CNormalEnemyFind()
{
}

void CNormalEnemyFind::Enter()
{
    GetOwner()->GetComponent<CAnimator>()->Play(MONSTERANIM(L"Find"),false);
}

void CNormalEnemyFind::tick()
{
    if (GetOwner()->GetComponent<CAnimator>()->IsFinish())
    {
        ChangeState(L"ATTACK");
    }
}

void CNormalEnemyFind::Exit()
{
}