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
    if (m_FrmEnter && GetOwner()->Animator()->GetClipFrameIndex() >= 11)
    {
        // spawn ability bubble
        if (nullptr != m_Bubble && PLAYERFSM->GetCurAbilityIdx() != AbilityCopyType::SLEEP)
        {
            CGameObject* pBubble = m_Bubble->Instantiate();
            Vec3 InitPos = PLAYER->Transform()->GetWorldPos();
            Vec3 InitRot = PLAYER->Transform()->GetLocalRotation();
            InitPos += PLAYER->Transform()->GetLocalDir(DIR_TYPE::UP) * 100.f;
            InitPos -= PLAYER->Transform()->GetLocalDir(DIR_TYPE::FRONT) * 100.f;
            InitRot.x += XMConvertToRadians(-90.f);
            InitRot.y += XMConvertToRadians(180.f);

            CKirbyCopyAbilityScript* pAbility = (CKirbyCopyAbilityScript*)CScriptMgr::GetScript(KIRBYCOPYABILITYSCRIPT);
            pBubble->AddComponent(PLAYERFSM->GetCurHat()->MeshRender()->Clone());
            pBubble->AddComponent(pAbility);
            pBubble->Transform()->SetLocalPos(InitPos);
            pBubble->Transform()->SetLocalScale(Vec3(80.f));
            pBubble->Transform()->SetLocalRotation(InitRot);
            pAbility->SetAbilityType(PLAYERFSM->GetCurAbilityIdx());

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
        if (GetOwner()->CharacterController()->IsGrounded())
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
    PLAYERFSM->SetGlobalState(true);

    PLAYER->Animator()->Play(ANIMPREFIX("AbilityDump"), false, false, 1.f);
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Frown);

    m_FrmEnter = true;
}

void CKirbyDropAbility::Exit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERFSM->ClearYPressedTime();
    PLAYERFSM->ChangeAbilityCopy(AbilityCopyType::NORMAL);
}