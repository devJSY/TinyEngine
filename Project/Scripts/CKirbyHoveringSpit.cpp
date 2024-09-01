#include "pch.h"
#include "CKirbyHoveringSpit.h"

CKirbyHoveringSpit::CKirbyHoveringSpit()
    : m_SavedSpeed(0.f)
{
    m_SpitSmoke = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\SpitSmoke.pref");
}

CKirbyHoveringSpit::CKirbyHoveringSpit(const CKirbyHoveringSpit& _Origin)
    : m_SavedSpeed(0.f)
{
    m_SpitSmoke = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\SpitSmoke.pref");
}

CKirbyHoveringSpit::~CKirbyHoveringSpit()
{
}

void CKirbyHoveringSpit::tick()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"LANDING");
        }
        ChangeState(L"JUMP_FALL");
    }
}

void CKirbyHoveringSpit::Enter()
{
    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Spit.wav", 1, 50.f);

    GetOwner()->Animator()->Play(ANIMPREFIX("SpitAir"), false);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));

    if (!PLAYERCTRL->IsGround())
    {
        PLAYERCTRL->AddVelocity(Vec3(0.f, 1.f, 0.f));
    }
    Vec3 InputWorld = PLAYERCTRL->GetInputWorld();
    if (InputWorld.Length())
        PLAYERCTRL->ForceDir({ForceDirType::DEFORM, InputWorld, false});

    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(PLAYERUNIT->GetInitInfo().Speed / 3.f);

    PLAYERFSM->SetHovering(true);
    PLAYERFSM->SetDroppable(true);

    // spit
    if (m_SpitSmoke != nullptr)
    {
        CGameObject* SpitSmokeObj = m_SpitSmoke->Instantiate();

        Vec3 PlayerPos = PLAYER->Transform()->GetWorldPos();
        Vec3 PlayerDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);

        Vec3 SmokePos = PlayerPos + PlayerDir * 15.f;
        SmokePos.y += 15.f;

        SpitSmokeObj->Transform()->SetWorldPos(SmokePos);
        SpitSmokeObj->Transform()->SetDirection(PlayerDir);

        GamePlayStatic::SpawnGameObject(SpitSmokeObj, SpitSmokeObj->GetLayerIdx());
    }
}

void CKirbyHoveringSpit::Exit()
{
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    PLAYERFSM->SetHovering(false);
    PLAYERFSM->SetDroppable(false);
}