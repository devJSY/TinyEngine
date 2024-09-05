#include "pch.h"
#include "CKirbyAttackAirGuard.h"
#include "CMomentaryObjScript.h"

CKirbyAttackAirGuard::CKirbyAttackAirGuard()
    : m_KirbySwordShockWavePref(nullptr)
    , m_PrevGravity(0.f)
    , m_PlayTime(1.f)
{
    m_KirbySwordShockWavePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySwordShockWave.pref");
}

CKirbyAttackAirGuard::~CKirbyAttackAirGuard()
{
}

void CKirbyAttackAirGuard::tick()
{
    m_PlayTime -= DT;

    if (m_PlayTime < 0.f)
    {
        ChangeState(L"ATTACK_AIRGUARD_END");
    }
}

void CKirbyAttackAirGuard::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("ButterflyScalesAttack"), true, false, 1.5f, 0.2);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthOpen));
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    //@Effect Ãæ°ÝÆÄ & ¿Ö°î
    Vec3 Pos = PLAYER->Transform()->GetWorldPos();
    Vec3 RightDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

    // Right
    {
        CGameObject* pShockWaveObj = m_KirbySwordShockWavePref->Instantiate();

        CMomentaryObjScript* pMomentaryScript = new CMomentaryObjScript;
        pMomentaryScript->SetPlayTime(3.f);
        pShockWaveObj->AddComponent(pMomentaryScript);

        pShockWaveObj->Transform()->SetWorldPos(Pos + RightDir * 25.f + Vec3(0.f, 25.f, 0.f));
        GamePlayStatic::SpawnGameObject(pShockWaveObj, LAYER_PLAYERATK_TRIGGER);
    }

    // Left
    {
        CGameObject* pShockWaveObj = m_KirbySwordShockWavePref->Instantiate();

        CMomentaryObjScript* pMomentaryScript = new CMomentaryObjScript;
        pMomentaryScript->SetPlayTime(3.f);
        pShockWaveObj->AddComponent(pMomentaryScript);

        pShockWaveObj->Transform()->SetWorldPos(Pos + -RightDir * 25.f + Vec3(0.f, 25.f, 0.f));
        GamePlayStatic::SpawnGameObject(pShockWaveObj, LAYER_PLAYERATK_TRIGGER);
    }

    PLAYERCTRL->ClearVelocityY();
    m_PrevGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    m_PlayTime = 1.f;

    // sound
    wstring Sound = L"sound\\wav\\HeroSwordMorpho\\ShockWave.wav";
    GamePlayStatic::Play2DSound(Sound, 1, SOUND_SWORDKIRBY * 1.2f);
}

void CKirbyAttackAirGuard::Exit()
{
    PLAYERCTRL->SetGravity(m_PrevGravity);
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();
}