#include "pch.h"
#include "CKirbyAttack.h"

CKirbyAttack::CKirbyAttack()
{
}

CKirbyAttack::~CKirbyAttack()
{
}

void CKirbyAttack::tick()
{
    PLAY_CURSTATE(Attack)

    //if (애니메이션 끝나면)
    //{
    //    // 내가 지금 무슨 모습인지에 따라 달라야됨
    //    switch (상태)
    //    {
    //    case 노말:
    //        break;

    //    case Fire:
    //        break;

    //    default:
    //        break;
    //    }
    //}
}

void CKirbyAttack::Enter()
{
    PLAY_CURSTATE(AttackEnter)
}

void CKirbyAttack::Exit()
{
    PLAY_CURSTATE(AttackExit)
}