#include "pch.h"
#include "CKirbyStuffedDamage.h"

CKirbyStuffedDamage::CKirbyStuffedDamage()
    : m_JumpPower(8.f)
    , m_InitSpeed(9.f)
    , m_AccTime(0.f)
    , m_Duration(0.65f)
{
}

CKirbyStuffedDamage::~CKirbyStuffedDamage()
{
}

void CKirbyStuffedDamage::tick()
{
    m_AccTime += DT;

    if (m_AccTime > m_Duration && PLAYERCTRL->IsGround())
    {
        PLAYERFSM->SetGlobalState(false);
        ChangeState(L"STUFFED_IDLE");
    }
}

void CKirbyStuffedDamage::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("StuffedDamage"), true, false, 1.5f);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));
    CPlayerMgr::SetPlayerFace(FaceType::Frown);

    Vec3 KnockBackDir = PLAYERUNIT->GetHitDir() * m_InitSpeed;
    KnockBackDir.y += m_JumpPower;
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->SetVelocity(KnockBackDir);
    PLAYERCTRL->SetFriction(0.f);
    PLAYERCTRL->SetFrictionMode(true);
    PLAYERCTRL->SetGravity(-35.f);

    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true, 3.f);
    PLAYERFSM->SetEmissive(true, 3.f);

    m_AccTime = 0.f;
}

void CKirbyStuffedDamage::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERCTRL->SetFriction(1.f);
    PLAYERCTRL->SetFrictionMode(false);
    PLAYERCTRL->SetGravity(-20.f);
}