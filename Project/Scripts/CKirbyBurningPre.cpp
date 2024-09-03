#include "pch.h"
#include "CKirbyBurningPre.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CRenderMgr.h>

CKirbyBurningPre::CKirbyBurningPre()
    : m_SaveGravity(0.f)
{
}

CKirbyBurningPre::~CKirbyBurningPre()
{
}

void CKirbyBurningPre::tick()
{
    if (PLAYER->GetChildObject(L"KirbyDragon")->Animator()->IsFinish())
    {
        ChangeState(L"BURNING_START");
    }
}

void CKirbyBurningPre::Enter()
{
    PLAYERFSM->ResetBurningParticleSpawn();

    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroFireDragon\\Fire_BurningPre.wav", 1, KIRBY_EFFECTSOUND);

    CGameObject* Wing = PLAYER->GetChildObject(L"KirbyDragon"); 

    if (Wing != nullptr)
    {
        Wing->SetActive(true);
    }

    Wing->Animator()->Play(ANIMPREFIX("BurningPre"), false, false, 1.5f);

    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("BurningStart"), false, false, 1.f);
    
    // Movement
    Vec3 Input = PLAYERCTRL->GetMoveDir();
    ForceDirInfo Info = {ForceDirType::DEFORM, Input, true};

    // 방향 강제 조정
    PLAYERCTRL->ForceDir(Info);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERCTRL->ClearVelocityY();
    m_SaveGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);


    //  무적 상태
    PLAYERFSM->SetInvincible(true);
}

void CKirbyBurningPre::Exit()
{
    CGameObject* Wing = PLAYER->GetChildObject(L"KirbyDragon");

    if (Wing != nullptr)
    {
        Wing->SetActive(false);
    }

    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERCTRL->SetGravity(m_SaveGravity);

    //  무적 상태
    PLAYERFSM->SetInvincible(false);
}
