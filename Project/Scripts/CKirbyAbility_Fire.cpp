#include "pch.h"
#include "CKirbyAbility_Fire.h"

#include "CPlayerMgr.h"
#include "CKirbyMoveController.h"
#include "CKirbyFSM.h"

#include <Engine/CMesh.h>

CKirbyAbility_Fire::CKirbyAbility_Fire()
    : m_bFrmEnter(true)
    , m_SavedSpeed(0.f)
{
}

CKirbyAbility_Fire::~CKirbyAbility_Fire()
{
}

// ===========
// 불 공격 시작
// ===========

void CKirbyAbility_Fire::AttackCharge1Start()
{


}

void CKirbyAbility_Fire::AttackCharge1StartEnter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"FireBreathStart"), false, false, 2.f);

    // 커비 표정 바꿔주기
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthOpen));
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    // Movement 제한 (방향, 점프)
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();

    // 속도 변경
    PLAYERCTRL->SetSpeed(2.f);
}

void CKirbyAbility_Fire::AttackCharge1StartExit()
{
    // 커비 표정 바꿔주기
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
    
    // Movement Unlock
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    // 속도 되돌려주기
    PLAYERCTRL->SetSpeed(m_SavedSpeed);
}

// ======
// 불 공격
// ======

void CKirbyAbility_Fire::AttackCharge1()
{
}

void CKirbyAbility_Fire::AttackCharge1Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"FireBreath"), true, false, 2.f);

    // 커비 표정 바꿔주기
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    // 커비 머터리얼 변경
    //CPlayerMgr::ClearBodyMtrl();
    //CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    // @TODO 움직임 구현 -> 회전값을 바꿔줘야 함
    // Movement 제한
    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(4.f);

    // @TODO 일정 주기마다 Fire Projectile 스폰
}

void CKirbyAbility_Fire::AttackCharge1Exit()
{
    // 커비 표정 바꿔주기
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    // 커비 머터리얼 변경
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));

    // Movement 복구
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);
}

// ========
// 공격 종료
// ========

void CKirbyAbility_Fire::AttackCharge1End()
{
}

void CKirbyAbility_Fire::AttackCharge1EndEnter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"FireBreathEnd"), false, false , 2.f);

    // Movement 제한
    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);
}

void CKirbyAbility_Fire::AttackCharge1EndExit()
{
    // Movement 복구
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

}

void CKirbyAbility_Fire::AttackCharge1Run()
{
}

void CKirbyAbility_Fire::AttackCharge1RunEnter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(KIRBYANIM(L"FireWalk"), true, false, 2.f);

    // Movement 제한
    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);

    // Kirby 표정 바꿔주기
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    // Kirby 머터리얼 변경
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyBig));

    // @TODO 일정 주기마다 Fire Projectile 스폰

}

void CKirbyAbility_Fire::AttackCharge1RunExit()
{
    // 커비 표정 바꿔주기
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    // Kirby 머터리얼 변경
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));

    // Movement 복구
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);
}

void CKirbyAbility_Fire::JumpAttack()
{
}

void CKirbyAbility_Fire::JumpAttackEnter()
{
}

void CKirbyAbility_Fire::JumpAttackExit()
{
}

void CKirbyAbility_Fire::ChangeAbility()
{
}

void CKirbyAbility_Fire::ChangeAbilityEnter()
{
    const vector<tBoneSocket*>& vecBoneSocket = PLAYER->Animator()->GetSkeletalMesh()->GetvecBoneSocket();

    for (tBoneSocket* BoneSocket : vecBoneSocket)
    {
        CGameObject* pInstObj = nullptr;

        if (BoneSocket->SoketName == L"HatLSocket_0")
        {
            pInstObj = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KiryDragonHat.pref", L"prefab\\KiryDragonHat.pref")->Instantiate();

            if (nullptr != pInstObj)
            {
                // 애니메이션 설정
                pInstObj->Animator()->Play(KIRBYANIM(L"Deform"));

                // 원점 설정
                pInstObj->Transform()->SetRelativePos(PLAYER->Transform()->GetWorldPos());
                pInstObj->Transform()->SetRelativeRotation(PLAYER->Transform()->GetWorldRotation());

                pInstObj->Transform()->SetAbsolute(false);


                GamePlayStatic::AddChildObject(PLAYER, pInstObj, BoneSocket);
            }
        }
    }
}

void CKirbyAbility_Fire::ChangeAbilityExit()
{
}
