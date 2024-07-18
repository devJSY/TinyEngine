#include "pch.h"
#include "CKirbyObject_Lightbulb.h"
#include "CKirbyUnitScript.h"

CKirbyObject_Lightbulb::CKirbyObject_Lightbulb()
    : m_Speed(8.f)
    , m_BrightSpeed(5.f)
    , m_PointLight(nullptr)
{
    m_OriginObject = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Lightbulb.pref", L"prefab\\Lightbulb.pref");
    m_Mesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyLightbulb.mdat", L"meshdata\\KirbyLightbulb.mdat");
    m_DemoMesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyLightbulbDemo.mdat", L"meshdata\\KirbyLightbulbDemo.mdat");
    
    ParseDemoMesh(m_DemoMesh);
}

CKirbyObject_Lightbulb::~CKirbyObject_Lightbulb()
{
}

// ===============
// 기본 상태들
// ===============

void CKirbyObject_Lightbulb::IdleEnter()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"WaitBright"));
        CPlayerMgr::SetPlayerFace(FaceType::Frown);
    }
    else
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"Wait"));
    }

    PLAYERCTRL->LockMove();
}

void CKirbyObject_Lightbulb::IdleExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
    PLAYERCTRL->UnlockMove();
}

void CKirbyObject_Lightbulb::RunEnter()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"MoveBright"), true, false, 1.5f, 0.3f);
        CPlayerMgr::SetPlayerFace(FaceType::Frown);
    }
    else
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"Move"), true, false, 1.5f, 0.3f);
    }
}

void CKirbyObject_Lightbulb::RunEndEnter()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"StopBright"), false);
        CPlayerMgr::SetPlayerFace(FaceType::Frown);
    }
    else
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"Stop"), false);
        CPlayerMgr::SetPlayerFace(FaceType::Close);
    }
}

void CKirbyObject_Lightbulb::RunEndExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
}

void CKirbyObject_Lightbulb::JumpStartEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Jump"), false);
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
}

void CKirbyObject_Lightbulb::LandingEnter()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
        CPlayerMgr::SetPlayerFace(FaceType::Frown);
        PLAYER->Animator()->Play(KIRBYANIM(L"LandingBright"), false);
    }
    else
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"Landing"), false);
    }

    PLAYERCTRL->LockJump();
}

void CKirbyObject_Lightbulb::LandingEndEnter()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"LandingEndBright"), false);
    }
    else
    {
        PLAYER->Animator()->Play(KIRBYANIM(L"LandingEnd"), false);
    }

    PLAYERCTRL->LockJump();
}

// ===============
// Attack
// ===============
// 불켜기/끄기

void CKirbyObject_Lightbulb::Attack()
{
}

void CKirbyObject_Lightbulb::AttackEnter()
{
    // Light켜기
    PLAYER->Animator()->Play(KIRBYANIM(L"LightOn"), false);
    CPlayerMgr::SetPlayerFace(FaceType::Frown);
    //@Effect 글로우 이펙트

    PLAYERCTRL->SetSpeed(m_BrightSpeed);
    PLAYERCTRL->SetRotSpeed(m_BrightSpeed);

    PLAYERFSM->SetAttackEvent(true);

    m_PointLight->SetActive(true);
}

void CKirbyObject_Lightbulb::AttackExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
}

// End
void CKirbyObject_Lightbulb::AttackEnd()
{
}

void CKirbyObject_Lightbulb::AttackEndEnter()
{
    // Light끄기
    PLAYER->Animator()->Play(KIRBYANIM(L"LightOff"), false);

    PLAYERCTRL->SetSpeed(m_Speed);
    PLAYERCTRL->SetRotSpeed(m_Speed);

    m_PointLight->SetActive(false);
}

void CKirbyObject_Lightbulb::AttackEndExit()
{
    PLAYERFSM->ClearChargeAccTime();
    PLAYERFSM->SetAttackEvent(false);
}

// ===============
// Drop Object
// ===============
void CKirbyObject_Lightbulb::DropObjectEnter()
{
    CKirbyObject::DropObjectEnter();

    // 콜라이더 & 바디콜라이더 크기 세팅
    CPlayerMgr::ResetBodyColliderSetting();

    UnitInfo PlayerInfo = PLAYERUNIT->GetInitInfo();
    PLAYERCTRL->SetSpeed(PlayerInfo.Speed);
    //PLAYERCTRL->SetRotSpeed(PlayerInfo.RotSpeed); //@TODO 머지후변경
    PLAYERCTRL->AddVelocity(Vec3(0.f, 3.5f, 0.f));

    // 포인트라이트
    m_PointLight->SetActive(false);
}

// ===============
// Change Object
// ===============
void CKirbyObject_Lightbulb::ChangeObjectEnter()
{
    CKirbyObject::ChangeObjectEnter();

    // 콜라이더 & 바디콜라이더 크기 세팅
    PLAYER->CharacterController()->SetCenter(Vec3(0.f, 145.f, 0.f));
    PLAYER->CharacterController()->SetRadius(1.25f);
    PLAYER->CharacterController()->SetHeight(4.f);

    CCapsuleCollider* BodyCol = PLAYER->GetChildObject(L"Body Collider")->CapsuleCollider();
    BodyCol->SetCenter(Vec3(0.f, 145.f, 0.f));
    BodyCol->SetRadius(1.25f);
    BodyCol->SetHeight(4.f);

    PLAYERCTRL->SetSpeed(m_Speed);
    PLAYERCTRL->SetRotSpeed(m_Speed);

    // PointLight
    m_PointLight = PLAYER->GetChildObject(L"DeformLight PointLight");
}
