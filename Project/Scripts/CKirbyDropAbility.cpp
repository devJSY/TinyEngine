#include "pch.h"
#include "CKirbyDropAbility.h"
#include "CMomentaryObjScript.h"
#include "CKirbyCopyAbilityScript.h"

CKirbyDropAbility::CKirbyDropAbility()
    : m_FrmEnter(true)
{
    m_Bubble = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyBubble.pref", L"prefab\\KirbyBubble.pref");
}

CKirbyDropAbility::~CKirbyDropAbility()
{
}

void CKirbyDropAbility::tick()
{
    // Change State
    // ---------------
    PLAYERFSM->GetPrevAbility()->DropAbility();

    // Common Process
    // ---------------
    if (m_FrmEnter && CHECK_ANIMFRM(GetOwner(), 11))
    {
        // spawn ability bubble
        if (nullptr != m_Bubble)
        {
            GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\DropAbility.wav", 1, KIRBY_EFFECTSOUND);

            CGameObject* pStarEffect =
                CAssetMgr::GetInst()
                    ->Load<CPrefab>(L"prefab\\Effect_KirbyChangeAbilityStarSpawn.pref", L"prefab\\Effect_KirbyChangeAbilityStarSpawn.pref")
                    ->Instantiate();

            GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());

            CGameObject* pBubble = m_Bubble->Instantiate();

            Vec3 InitPos = PLAYER->Transform()->GetWorldPos();
            Vec3 InitRot = PLAYER->Transform()->GetLocalRotation();
            float ScaleFactor = PLAYER->Transform()->GetLocalScale().x;
            InitPos += PLAYER->Transform()->GetLocalDir(DIR_TYPE::UP) * ScaleFactor;
            InitPos -= PLAYER->Transform()->GetLocalDir(DIR_TYPE::FRONT) * ScaleFactor;
            pBubble->Transform()->SetLocalPos(InitPos);
            pBubble->Transform()->SetLocalRotation(InitRot);

            CKirbyCopyAbilityScript* pAbility = (CKirbyCopyAbilityScript*)CScriptMgr::GetScript(KIRBYCOPYABILITYSCRIPT);
            pAbility->SetAbilityType(PLAYERFSM->GetPrevAbilityIdx());
            pBubble->AddComponent(pAbility);

            CGameObject* pMesh = new CGameObject;
            pMesh->SetName(L"Mesh");
            pMesh->AddComponent(new CTransform);
            pMesh->AddComponent(PLAYERFSM->GetCurHat()->MeshRender()->Clone());
            if (PLAYERFSM->GetCurHat()->Animator())
            {
                pMesh->AddComponent(PLAYERFSM->GetCurHat()->Animator()->Clone());
                pMesh->Animator()->Play(ANIMPREFIX("Deform"));
            }
            else
            {
                pMesh->Transform()->SetLocalRotation(Vec3(XMConvertToRadians(-90.f), XMConvertToRadians(180.f), 0.f));
            }

            pMesh->Transform()->SetAbsolute(false);
            pMesh->Transform()->SetLocalPos(Vec3(0.f, -0.4f, 0.f));
            pMesh->Transform()->SetLocalScale(Vec3(0.9f));
            pMesh->MeshRender()->SetCastShadow(false);

            GamePlayStatic::AddChildObject(pBubble, pMesh);

            CMomentaryObjScript* pMomentaryObj = pBubble->GetScript<CMomentaryObjScript>();
            if (nullptr != pMomentaryObj)
            {
                pMomentaryObj->SetInitVelocity(Vec3(0.f, 10.f, 0.f));
                pMomentaryObj->SetPlayTime(10.f);
            }

            GamePlayStatic::SpawnGameObject(pBubble, LAYER_DYNAMIC);
        }

        // clear current hat & weapon
        PLAYERFSM->ClearCurHatWeapon();

        m_FrmEnter = false;
    }

    // Change State
    if (GetOwner()->Animator()->IsFinish())
    {
        PLAYERFSM->SetGlobalState(false);
        if (PLAYERCTRL->IsGround())
        {
            ChangeState(L"IDLE");
        }
        else
        {
            ChangeState(L"JUMP_FALL");
        }
    }
}

void CKirbyDropAbility::Enter()
{
    // Change State
    // ---------------
    PLAYERFSM->GetCurAbility()->DropAbilityEnter();

    // Common Process
    // ---------------
    PLAYERFSM->SetGlobalState(true);
    PLAYERFSM->SetInvincible(true);
    PLAYERFSM->ChangeAbilityCopy(AbilityCopyType::NORMAL);

    PLAYER->Animator()->Play(ANIMPREFIX("AbilityDump"), false, false, 1.f);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Frown);

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockMove();

    m_FrmEnter = true;
}

void CKirbyDropAbility::Exit()
{


    // Change State
    // ---------------
    PLAYERFSM->GetPrevAbility()->DropAbilityExit();

    // Common Process
    // ---------------
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockMove();

    PLAYERFSM->SetInvincible(false);
    PLAYERFSM->ClearYPressedTime();
}