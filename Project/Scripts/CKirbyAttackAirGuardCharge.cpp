#include "pch.h"
#include "CKirbyAttackAirGuardCharge.h"

CKirbyAttackAirGuardCharge::CKirbyAttackAirGuardCharge()
    : m_PrevGravity(0.f)
    , m_AccTime(0.f)
    , m_PlayTime(0.5f)
    , m_bEnter(false)
{
}

CKirbyAttackAirGuardCharge::~CKirbyAttackAirGuardCharge()
{
}

void CKirbyAttackAirGuardCharge::tick()
{
    m_AccTime += DT;
    if (m_bEnter && m_AccTime >= m_PlayTime)
    {
        //@Effect 차징완료 이펙트
        PLAYERCTRL->SetGravity(-0.5f);
    }

    if (KEY_RELEASED(KEY_GUARD) || KEY_NONE(KEY_GUARD) || KEY_RELEASED(KEY_ATK) || KEY_NONE(KEY_ATK))
    {
        if (m_AccTime < m_PlayTime)
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
    PLAYER->Animator()->Play(ANIMPREFIX("ButterflyScalesCharge"), true, false, 1.5f);
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

    // sound
    wstring Charge = L"sound\\wav\\HeroSwordCharge\\Charge_Repeat.wav";
    wstring Sound = L"sound\\wav\\HeroSwordMorpho\\ShockWave_ChargeFinish.wav";
    GamePlayStatic::Play2DSound(Charge, 0, SOUND_SWORDKIRBY * 0.6f, true, false);
    GamePlayStatic::Play2DSound(Sound, 1, SOUND_SWORDKIRBY);
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

    // sound
    wstring Charge = L"sound\\wav\\HeroSwordCharge\\Charge_Repeat.wav";
    GamePlayStatic::StopSound(Charge);
}