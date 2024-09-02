#include "pch.h"
#include "CKirbyAbility_Normal.h"
#include "CKirbyBulletScript.h"
#include "CKirbyMoveController.h"
#include "CKirbyVacuumCollider.h"
#include "CUnitScript.h"

CKirbyAbility_Normal::CKirbyAbility_Normal()
    : m_bFrmEnter(true)
    , m_SavedSpeed(0.f)
{
    m_Charge1Time = 2.f;
}

CKirbyAbility_Normal::CKirbyAbility_Normal(const CKirbyAbility_Normal& _Origin)
    : CKirbyAbility(_Origin)
    , m_bFrmEnter(true)
    , m_SavedSpeed(0.f)
{
    m_Charge1Time = 2.f;
}

CKirbyAbility_Normal::~CKirbyAbility_Normal()
{
}

// ===============
// Attack
// ===============
// 머금은 물체 뱉기
// - Stuffed일 때만 들어옴

void CKirbyAbility_Normal::Attack()
{
    if (CHECK_ANIMFRM(PLAYER, 3) && m_bFrmEnter)
    {
        CPlayerMgr::ClearBodyMtrl();
        CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));
        m_bFrmEnter = false;

        // fire bullet
        Ptr<CPrefab> BulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyBullet.pref", L"prefab\\KirbyBullet.pref");
        if (nullptr != BulletPref)
        {
            CGameObject* BulletInst = BulletPref->Instantiate();
            Vec3 InitPos = PLAYER->Transform()->GetWorldPos();
            InitPos += PLAYERCTRL->GetInputWorld() * 3.f * PLAYER->Transform()->GetLocalScale();
            InitPos += PLAYER->Transform()->GetWorldDir(DIR_TYPE::UP) * 2.f * PLAYER->Transform()->GetLocalScale();

            BulletInst->Transform()->SetLocalPos(InitPos);
            GamePlayStatic::SpawnGameObject(BulletInst, LAYER_PLAYERATK);

            CKirbyBulletScript* bulletScript = BulletInst->GetScript<CKirbyBulletScript>();
            if (nullptr != bulletScript)
            {
                Vec3 InitDir = PLAYERCTRL->GetInputWorld();
                InitDir.y = 0.f;
                bulletScript->SetInitVelocity(InitDir * 30.f);

                PLAYERCTRL->ForceDir({ForceDirType::STATE, InitDir, true});
            }
        }
    }
}

void CKirbyAbility_Normal::AttackEnter()
{
    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\AttackNormal.wav", 1, KIRBY_EFFECTSOUND);

    if (PLAYERCTRL->IsGround())
    {
        PLAYER->Animator()->Play(ANIMPREFIX("Spit"), false);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX("SpitAir"), false);
    }

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();

    PLAYERFSM->SetUnstuffReverse(false);

    m_bFrmEnter = true;
}

void CKirbyAbility_Normal::AttackExit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();

    PLAYERFSM->SetUnstuffReverse(true);
}

// ===============
// Change Ability
// ===============
void CKirbyAbility_Normal::ChangeAbility()
{
}

void CKirbyAbility_Normal::ChangeAbilityEnter()
{
    PLAYERFSM->SetCurHat(nullptr);
    PLAYERFSM->SetCurWeapon(nullptr);
}

void CKirbyAbility_Normal::ChangeAbilityExit()
{
}
