#include "pch.h"
#include "CKirbyObject_Cone.h"
#include "CKirbyUnitScript.h"

CKirbyObject_Cone::CKirbyObject_Cone()
    : m_bFrmEnter(true)
{
    m_OriginObject = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Cone.pref", L"prefab\\Cone.pref");
    m_Mesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyCone.mdat", L"meshdata\\KirbyCone.mdat");
    m_DemoMesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyConeDemo.mdat", L"meshdata\\KirbyConeDemo.mdat");
    m_Crater = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Crater.pref", L"prefab\\Crater.pref");

    ParseDemoMesh(m_DemoMesh);
}

CKirbyObject_Cone::~CKirbyObject_Cone()
{
}

// ===============
// Attack
// ===============
// 내려찍기 - 시작

// (떨어지기 시작)
void CKirbyObject_Cone::Attack()
{
}

void CKirbyObject_Cone::AttackEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("TurnEnd"), false, false, 1.5f);

    PLAYERCTRL->ClearVelocityY();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
}

void CKirbyObject_Cone::AttackExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
}

// Start (위로 휘리릭)
void CKirbyObject_Cone::AttackStart()
{
}

void CKirbyObject_Cone::AttackStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Turn"), false, false, 1.5f);

    // 속도세팅
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->AddVelocity(Vec3(0.f, 15.f, 0.f));
}

void CKirbyObject_Cone::AttackStartExit()
{
    PLAYERCTRL->UnlockDirection();
}

// End (떨어지는 중)
void CKirbyObject_Cone::AttackEnd()
{
}

void CKirbyObject_Cone::AttackEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("TurnEnd"), true, false, 1.5f);
    //@Effect 속도선

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->AddVelocity(Vec3(0.f, -20.f, 0.f));

    PLAYERFSM->SetInvincible(true);
}

void CKirbyObject_Cone::AttackEndExit()
{
    PLAYERCTRL->UnlockDirection();

    PLAYERFSM->SetInvincible(false);
}

// ===============
// Attack
// ===============
// 내려찍기 - 끝

// (충돌)
void CKirbyObject_Cone::AttackCombo1()
{
}

#include "CKirbyBulletScript.h"

void CKirbyObject_Cone::AttackCombo1Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("TurnLanding"), false, false, 1.5f);
    //@Effect 충돌이펙트

    // Crater 소환
    {
        CGameObject* pCrater = m_Crater->Instantiate();
        CKirbyBulletScript* pBulletScript = pCrater->GetScript<CKirbyBulletScript>();
        Vec3 InitPos = PLAYER->CharacterController()->GetFootPos();
        
        pCrater->Transform()->SetLocalPos(InitPos);
        pBulletScript->SetInitVelocity(Vec3());
        pBulletScript->SetPlayTime(10.f);
        pBulletScript->SetEndAnim(ANIMPREFIX("End"));
        pCrater->Animator()->Play(ANIMPREFIX("Start"), false);

        GamePlayStatic::SpawnGameObject(pCrater, LAYER_PLAYERATK);
    }

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();

    PLAYERFSM->SetInvincible(true);
}

void CKirbyObject_Cone::AttackCombo1Exit()
{
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->SetInvincible(false);
}

// End (일어남)
void CKirbyObject_Cone::AttackCombo1End()
{
}

void CKirbyObject_Cone::AttackCombo1EndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("TurnEndEnd"), false, false, 1.5f);
    //@Effect 충돌이펙트

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
}

void CKirbyObject_Cone::AttackCombo1EndExit()
{
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
}

// ===============
// Drop Object
// ===============

void CKirbyObject_Cone::DropObjectEnter()
{
    CKirbyObject::DropObjectEnter();
    UnitInfo PlayerInfo = PLAYERUNIT->GetInitInfo();
    PLAYERCTRL->SetSpeed(PlayerInfo.Speed);
    PLAYERCTRL->SetRotSpeed(PlayerInfo.RotationSpeed);

    // 콜라이더 & 바디콜라이더 크기 세팅
    CPlayerMgr::ResetBodyColliderSetting();
}

// ===============
// Change Object
// ===============

void CKirbyObject_Cone::ChangeObjectEnter()
{
    CKirbyObject::ChangeObjectEnter();

    PLAYERCTRL->SetSpeed(7.f);
    PLAYERCTRL->SetSpeed(5.f);

    // 콜라이더 & 바디콜라이더 크기 세팅
    PLAYER->CharacterController()->SetCenter(Vec3(0.f, 140.f, 0.f));
    PLAYER->CharacterController()->SetRadius(1.5f);
    PLAYER->CharacterController()->SetHeight(4.5f);

    CCapsuleCollider* BodyCol = PLAYER->GetChildObject(L"Body Collider")->CapsuleCollider();
    BodyCol->SetCenter(Vec3(0.f, 140.f, 0.f));
    BodyCol->SetRadius(1.5f);
    BodyCol->SetHeight(4.5f);
}
