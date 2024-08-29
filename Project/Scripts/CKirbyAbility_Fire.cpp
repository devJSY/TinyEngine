#include "pch.h"
#include "CKirbyAbility_Fire.h"

#include "CPlayerMgr.h"
#include "CKirbyMoveController.h"
#include "CKirbyFSM.h"
#include "CKirbyFireBullet.h"

#include <Engine/CMesh.h>

CKirbyAbility_Fire::CKirbyAbility_Fire()
    : m_bFrmEnter(true)
    , m_SavedSpeed(0.f)
    , m_ProjectileAcc(0.f)
    , m_ProjectileCooltime(0.1f)
{
    m_Hat = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KiryDragonHat.pref", L"prefab\\KiryDragonHat.pref");
}

CKirbyAbility_Fire::CKirbyAbility_Fire(const CKirbyAbility_Fire& _Origin)
    : CKirbyAbility(_Origin)
    , m_bFrmEnter(true)
    , m_SavedSpeed(0.f)
    , m_ProjectileAcc(0.f)
    , m_ProjectileCooltime(_Origin.m_ProjectileCooltime)
{
    m_Hat = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KiryDragonHat.pref", L"prefab\\KiryDragonHat.pref");
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
    // Projectile CoolTime, Acc 초기화
    m_ProjectileAcc = 0.f;
    m_ProjectileCooltime = 0.1f;

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("FireBreathStart"), false, false, 2.f);

    // 커비 표정 바꿔주기
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthOpen));
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    // Movement 제한 (방향, 점프)
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();

    // 속도 변경
    PLAYERCTRL->SetSpeed(m_SavedSpeed/2.f);
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
    // 키보드 입력이 없으면 해당 방향에서 멈추도록 설정
    if (PLAYERCTRL->GetInput().Length() == 0.f)
    {
        PLAYERCTRL->LockDirection();
    }
    else
    {
        PLAYERCTRL->UnlockDirection();
    }


    // tick
    m_ProjectileAcc += DT;

    if (m_ProjectileAcc > m_ProjectileCooltime)
    {
        // Fire Projectile 스폰
        Ptr<CPrefab> FireProjectilePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FireBulletLV0.pref");
        CGameObject* FireProjectileObj = FireProjectilePref->Instantiate();

        // Kirby의 입쪽에서 FireProjectile이 스폰되도록 위치를 계산한다. 방향은 커비가 바라보고 있는 방향으로 한다.
        Vec3 KirbyPos = PLAYER->Transform()->GetWorldPos();
        Vec3 KirbyFront = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 Offset = Vec3(0.f, 25.f, 0.f) + KirbyFront * 5.f;

        FireProjectileObj->Transform()->SetWorldPos(KirbyPos + Offset);
        FireProjectileObj->Transform()->SetDirection(KirbyFront);
        FireProjectileObj->GetScript<CKirbyFireBullet>()->SetFirst();

        GamePlayStatic::SpawnGameObject(FireProjectileObj, FireProjectileObj->GetLayerIdx());

        m_ProjectileAcc -= m_ProjectileCooltime;
    }
}

void CKirbyAbility_Fire::AttackCharge1Enter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("FireBreath"), true, false, 2.f);

    // 커비 표정 바꿔주기
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    // Movement 제한
    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(4.f);

    // 회전 속도 제한
    m_SaveRotSpeed = PLAYERCTRL->GetRotSpeed();
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed / 4.f);
    
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
    PLAYERCTRL->UnlockDirection();

    // 회전 속도 복구
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed);
}

// ========
// 공격 종료
// ========

void CKirbyAbility_Fire::AttackCharge1End()
{
}

void CKirbyAbility_Fire::AttackCharge1EndEnter()
{
    m_ProjectileAcc = 0.f;

    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("FireBreathEnd"), false, false , 2.f);

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
    // 키보드 입력이 없으면 해당 방향에서 멈추도록 설정
    if (PLAYERCTRL->GetInput().Length() == 0.f)
    {
        PLAYERCTRL->LockDirection();
    }
    else
    {
        PLAYERCTRL->UnlockDirection();
    }

    // tick
    m_ProjectileAcc += DT;

    if (m_ProjectileAcc > m_ProjectileCooltime)
    {
        // Fire Projectile 스폰
        Ptr<CPrefab> FireProjectilePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FireBulletLV0.pref");
        CGameObject* FireProjectileObj = FireProjectilePref->Instantiate();

        // Kirby의 입쪽에서 FireProjectile이 스폰되도록 위치를 계산한다. 방향은 커비가 바라보고 있는 방향으로 한다.
        Vec3 KirbyPos = PLAYER->Transform()->GetWorldPos();
        Vec3 KirbyFront = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 Offset = Vec3(0.f, 25.f, 0.f) + KirbyFront * 5.f;

        FireProjectileObj->Transform()->SetWorldPos(KirbyPos + Offset);
        FireProjectileObj->Transform()->SetDirection(KirbyFront);
        FireProjectileObj->GetScript<CKirbyFireBullet>()->SetFirst();

        GamePlayStatic::SpawnGameObject(FireProjectileObj, FireProjectileObj->GetLayerIdx());

        m_ProjectileAcc -= m_ProjectileCooltime;
    }
}

void CKirbyAbility_Fire::AttackCharge1RunEnter()
{
    // 애니메이션 재생
    PLAYER->Animator()->Play(ANIMPREFIX("FireWalk"), true, false, 2.f);

    // Movement 제한
    PLAYERCTRL->LockJump();
    m_SavedSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(2.f);

    // Kirby 표정 바꿔주기
    CPlayerMgr::SetPlayerFace(FaceType::UpTail);

    
    // 회전 속도 제한
    m_SaveRotSpeed = PLAYERCTRL->GetRotSpeed();
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed / 3.f);

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
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->SetSpeed(m_SavedSpeed);

    // 회전 속도 복구
    PLAYERCTRL->SetRotSpeed(m_SaveRotSpeed);
}


void CKirbyAbility_Fire::ChangeAbility()
{
}

void CKirbyAbility_Fire::ChangeAbilityEnter()
{
    // 소켓에 모자 끼워주기
    if (m_Hat.Get())
    {
        CGameObject* pInstObj = m_Hat->Instantiate();
        GamePlayStatic::AddChildObject(PLAYER, pInstObj, L"Hat");
        PLAYERFSM->SetCurHat(pInstObj);

        // 애니메이션 재생
        pInstObj->Animator()->Play(ANIMPREFIX("Deform"),false,false,2.5f,0);
    }
}

void CKirbyAbility_Fire::ChangeAbilityExit()
{
}
