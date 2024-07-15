#include "pch.h"
#include "CKirbyAttackAirGuardCharge.h"
#define PLAYTIME 0.5f

CKirbyAttackAirGuardCharge::CKirbyAttackAirGuardCharge()
    : m_PrevGravity(0.f)
    , m_AccTime(0.f)
    , m_bEnter(false)
{
}

CKirbyAttackAirGuardCharge::~CKirbyAttackAirGuardCharge()
{
}

void CKirbyAttackAirGuardCharge::tick()
{
    m_AccTime += DT;
    if (m_bEnter && m_AccTime >= PLAYTIME)
    {
        //@Effect 차징완료 이펙트
        PLAYERCTRL->SetGravity(-0.5f);
    }

    if (KEY_RELEASED(KEY_GUARD) || KEY_NONE(KEY_GUARD) || KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
    {
        if (m_AccTime < PLAYTIME)
        {
            ChangeState(L"JUMP_FALL");
        }
        else
        {
            ChangeState(L"ATTACK_AIRGUARD");
        }
    }
}

void CKirbyAttackAirGuardCharge::Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"ButterflyScalesCharge"), true, false, 1.5f);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthOpen));
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);
    //@Effect 차지 파티클입자

    PLAYERCTRL->ClearVelocityY();
    m_PrevGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    m_AccTime = 0.f;
    m_bEnter = true;
}

void CKirbyAttackAirGuardCharge::Exit()
{
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->SetGravity(m_PrevGravity);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();
}