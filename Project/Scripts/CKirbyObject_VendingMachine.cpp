#include "pch.h"
#include "CKirbyObject_VendingMachine.h"

#include "CPlayerMgr.h"

#include "CKirbyUnitScript.h"

CKirbyObject_VendingMachine::CKirbyObject_VendingMachine()
{
    m_OriginObject = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\VendingMachine.pref", L"prefab\\VendingMachine.pref");
    m_Mesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyVendingMachine.mdat", L"meshdata\\KirbyVendingMachine.mdat");
    m_DemoMesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyVendingMachineDemo.mdat", L"meshdata\\KirbyVendingMachineDemo.mdat");

    ParseDemoMesh(m_DemoMesh);
}

CKirbyObject_VendingMachine::~CKirbyObject_VendingMachine()
{
}

void CKirbyObject_VendingMachine::AttackStart()
{
    if (PLAYER->Animator()->IsFinish())
    {
        if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->GetCanCount())
        {
            ChangeState(L"ATTACK_START");
        }
        else
        {
            ChangeState(L"ATTACK_END");
        }
    }


}

void CKirbyObject_VendingMachine::AttackStartEnter()
{
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();

    // Can Counting
    PLAYERFSM->SubCanCount();

    // Animation
    PLAYER->Animator()->Play(ANIMPREFIX("Shoot"), false, false, 2.f, 0);

    // Bullet
    Vec3 KirbyPos = PLAYER->Transform()->GetWorldPos();
    Vec3 KirbyWorldDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 Offset = Vec3(0.f, 50.f, 0.f);

    // 프리팹 생성
    Ptr<CPrefab> CanJuice = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\CanJuice.pref");
    CGameObject* CanJuiceInst = CanJuice->Instantiate();

    // 초기값 세팅
    // 예외처리 Dir 이 Vec3(0.f, 0.f, -1.f)인경우 Up벡터가 반전됨
    Vec3 up = Vec3(0.f, 1.f, 0.f);
    if (KirbyWorldDir == Vec3(0.f, 0.f, -1.f))
    {
        up = Vec3(0.f, -1.f, 0.f);
    }

    CanJuiceInst->Transform()->SetDirection(-KirbyWorldDir, up);
    CanJuiceInst->Transform()->SetWorldPos(KirbyPos + Offset);

    GamePlayStatic::SpawnGameObject(CanJuiceInst, CanJuiceInst->GetLayerIdx());
}

void CKirbyObject_VendingMachine::AttackStartExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();

}

void CKirbyObject_VendingMachine::AttackEnd()
{
    if (PLAYER->Animator()->IsFinish())
    {
        if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
        {
            ChangeState(L"ATTACK_FAILED");
        }
        else
        {
            ChangeState(L"IDLE");
        }
    }
}

void CKirbyObject_VendingMachine::AttackEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("ShootEnd"), false, false, 2.f, 0);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();
}

void CKirbyObject_VendingMachine::AttackEndExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();
}

// ===============
// Drop Object
// ===============

void CKirbyObject_VendingMachine::DropObjectEnter()
{
    CKirbyObject::DropObjectEnter();
    UnitInfo PlayerInfo = PLAYERUNIT->GetInitInfo();
    PLAYERCTRL->SetSpeed(PlayerInfo.Speed);
    PLAYERCTRL->SetSpeed(10.f);
    PLAYERCTRL->SetJumpPower(m_SaveJumpPower);
    // PLAYERCTRL->SetRotSpeed(PlayerInfo.RotSpeed); //@TODO 머지후변경

    // 콜라이더 & 바디콜라이더 크기 세팅
    CPlayerMgr::ResetBodyColliderSetting();
}

// ===============
// Change Object
// ===============
void CKirbyObject_VendingMachine::ChangeObjectEnter()
{
    CKirbyObject::ChangeObjectEnter();

    PLAYERCTRL->SetSpeed(5.f);
    m_SaveJumpPower = PLAYERCTRL->GetJumpPower();
    PLAYERCTRL->SetJumpPower(m_SaveJumpPower/2.f);

    // 콜라이더 & 바디콜라이더 크기 세팅
    PLAYER->CharacterController()->SetCenter(Vec3(0.f, 200.f, 0.f));
    PLAYER->CharacterController()->SetRadius(1.2f);
    PLAYER->CharacterController()->SetHeight(4.5f);

    CCapsuleCollider* BodyCol = PLAYER->GetChildObject(L"Body Collider")->CapsuleCollider();
    BodyCol->SetCenter(Vec3(0.f, 200.f, 0.f));
    BodyCol->SetRadius(1.2f);
    BodyCol->SetHeight(4.5f);

    // CanJuice 개수 초기화
    PLAYERFSM->ResetCanCount();
}
