#include "pch.h"
#include "CKirbyDropAbility.h"
#include "CKirbyBulletScript.h"

CKirbyDropAbility::CKirbyDropAbility()
    : m_FrmEnter(true)
{
    m_Bullet = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyBullet.pref", L"prefab\\KirbyBullet.pref");
}

CKirbyDropAbility::~CKirbyDropAbility()
{
}

void CKirbyDropAbility::tick()
{
    if (m_FrmEnter && GetOwner()->Animator()->GetClipFrameIndex() == 11)
    {
        // spawn ability buble
        if (nullptr != m_Bullet)
        {
            CGameObject* pBubble = m_Bullet->Instantiate();
            Vec3 InitPos = PLAYER->Transform()->GetWorldPos();
            InitPos += PLAYER->Transform()->GetLocalDir(DIR_TYPE::UP) * 100.f;
            InitPos -= PLAYER->Transform()->GetLocalDir(DIR_TYPE::FRONT) * 100.f;

            pBubble->AddComponent(PLAYERFSM->GetCurHat()->MeshRender()->Clone());
            pBubble->Transform()->SetLocalPos(InitPos);
            pBubble->Transform()->SetLocalScale(Vec3(80.f));

            CKirbyBulletScript* bulletScript = pBubble->GetScript<CKirbyBulletScript>();
            if (nullptr != bulletScript)
            {
                bulletScript->SetInitVelocity(Vec3(0.f, 10.f, 0.f));
                bulletScript->SetPlayTime(10.f);
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
    PLAYER->Animator()->Play(KIRBYANIM(L"AbilityDump"), false, false, 1.f);
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