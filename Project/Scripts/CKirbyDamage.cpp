#include "pch.h"
#include "CKirbyDamage.h"
#include "CPlayerMgr.h"
#include "CKirbyUnitScript.h"

CKirbyDamage::CKirbyDamage()
    : m_Acc(0.f)
    , m_Duration(0.05f)
{
}

CKirbyDamage::~CKirbyDamage()
{
}

void CKirbyDamage::tick()
{
    m_Acc += DT;

    if (m_Duration < m_Acc)
    {
        PLAYERFSM->SetGlobalState(false);
        ChangeState(L"BACKJUMP");
    }
}

void CKirbyDamage::Enter()
{
    PLAYERFSM->SetGlobalState(true);

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("Damage"), true, false, 1.f);
    CPlayerMgr::SetPlayerFace(FaceType::Frown);
    if (PLAYERFSM->GetCurObjectIdx() == ObjectCopyType::NONE)
    {
        CPlayerMgr::ClearMouthMtrl();
        CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthAngryClose));
    }

    // 방향, 점프, 이동 Lock
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERCTRL->SetVelocity({0.f, 0.f, 0.f});

    // Acc 초기화
    m_Acc = 0.f;

    // 무적, 이미시브 효과
    PLAYERFSM->SetInvincible(true, 3.f);
    PLAYERFSM->SetEmissive(true, 3.f);

    PLAYERFSM->SetKnockBackDir(PLAYERUNIT->GetHitDir());
}

void CKirbyDamage::Exit()
{
    // 방향, 점프, 이동 Unlock
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

