#include "pch.h"
#include "CKirbyObject_Lightbulb.h"
#include "CKirbyUnitScript.h"
#include "CKirbyLightScript.h"

CKirbyObject_Lightbulb::CKirbyObject_Lightbulb()
    : m_Speed(8.f)
    , m_BrightSpeed(3.f)
    , m_PointLight(nullptr)
{
    m_OriginObject = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Lightbulb.pref", L"prefab\\Lightbulb.pref");
    m_Mesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyLightbulb.mdat", L"meshdata\\KirbyLightbulb.mdat");
    m_DemoMesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyLightbulbDemo.mdat", L"meshdata\\KirbyLightbulbDemo.mdat");

    ParseDemoMesh(m_DemoMesh);
}

CKirbyObject_Lightbulb::CKirbyObject_Lightbulb(const CKirbyObject_Lightbulb& _Origin)
    : CKirbyObject(_Origin)
    , m_Speed(8.f)
    , m_BrightSpeed(3.f)
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
        PLAYER->Animator()->Play(ANIMPREFIX("WaitBright"));
        CPlayerMgr::SetPlayerFace(FaceType::Frown);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX("Wait"));
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
        PLAYER->Animator()->Play(ANIMPREFIX("MoveBright"), true, false, 1.5f, 0.3f);
        CPlayerMgr::SetPlayerFace(FaceType::Frown);
        PLAYERCTRL->SetSpeed(m_BrightSpeed);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX("Move"), true, false, 1.5f, 0.3f);
        PLAYERCTRL->SetSpeed(m_Speed);
    }
}

void CKirbyObject_Lightbulb::RunExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
}

void CKirbyObject_Lightbulb::RunEndEnter()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
        PLAYER->Animator()->Play(ANIMPREFIX("StopBright"), false, false, 3.f);
        CPlayerMgr::SetPlayerFace(FaceType::Frown);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX("Stop"), false, false, 3.f);
        CPlayerMgr::SetPlayerFace(FaceType::Close);
    }

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
}

void CKirbyObject_Lightbulb::RunEndExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
}

void CKirbyObject_Lightbulb::JumpStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Jump"), false);
}

void CKirbyObject_Lightbulb::LandingEnter()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
        CPlayerMgr::SetPlayerFace(FaceType::Frown);
        PLAYER->Animator()->Play(ANIMPREFIX("LandingBright"), false);
        PLAYERCTRL->SetSpeed(m_BrightSpeed / 2.f);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX("Landing"), false);
        PLAYERCTRL->SetSpeed(m_Speed / 2.f);
    }

    PLAYERCTRL->LockJump();
}

void CKirbyObject_Lightbulb::LandingExit()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
    PLAYERCTRL->SetSpeed(m_BrightSpeed);
    }
    else
    {
    PLAYERCTRL->SetSpeed(m_Speed);
    }

    CPlayerMgr::SetPlayerFace(FaceType::Normal);
    PLAYERCTRL->UnlockJump();
}

void CKirbyObject_Lightbulb::LandingEndEnter()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
        PLAYER->Animator()->Play(ANIMPREFIX("LandingEndBright"), false);
    PLAYERCTRL->SetSpeed(m_BrightSpeed / 2.f);
    }
    else
    {
        PLAYER->Animator()->Play(ANIMPREFIX("LandingEnd"), false);
    PLAYERCTRL->SetSpeed(m_Speed / 2.f);
    }

    PLAYERCTRL->LockJump();
}

void CKirbyObject_Lightbulb::LandingEndExit()
{
    if (KEY_PRESSED(KEY_ATK) && PLAYERFSM->IsAttackEvent())
    {
    PLAYERCTRL->SetSpeed(m_BrightSpeed);
    }
    else
    {
    PLAYERCTRL->SetSpeed(m_Speed);
    }
    PLAYERCTRL->UnlockJump();
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
    PLAYER->Animator()->Play(ANIMPREFIX("LightOn"), false, false, 2.5f, 0.2);
    CPlayerMgr::SetPlayerFace(FaceType::Frown);
    //@Effect 글로우 이펙트

    PLAYERCTRL->SetSpeed(m_BrightSpeed);
    PLAYERCTRL->SetRotSpeed(m_BrightSpeed);
    PLAYERCTRL->LockMove();

    PLAYERFSM->SetAttackEvent(true);

    if (m_PointLight)
    {
        m_PointLight->TurnOn();
    }
}

void CKirbyObject_Lightbulb::AttackExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);
    PLAYERCTRL->UnlockMove();
}

// End
void CKirbyObject_Lightbulb::AttackEnd()
{
}

void CKirbyObject_Lightbulb::AttackEndEnter()
{
    // Light끄기
    PLAYER->Animator()->Play(ANIMPREFIX("LightOff"), false);

    if (m_PointLight)
    {
        m_PointLight->TurnOff();
    }
}

void CKirbyObject_Lightbulb::AttackEndExit()
{
    PLAYERCTRL->SetSpeed(m_Speed);
    PLAYERCTRL->SetRotSpeed(m_Speed);
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
    PLAYERCTRL->SetRotSpeed(PlayerInfo.RotationSpeed);
    PLAYERCTRL->AddVelocity(Vec3(0.f, 3.5f, 0.f));

    // 포인트라이트
    if (m_PointLight)
    {
        m_PointLight->TurnOff();
    }
}

// ===============
// Change Object
// ===============
void CKirbyObject_Lightbulb::ChangeObjectEnter()
{
    CKirbyObject::ChangeObjectEnter();

    // 콜라이더 & 바디콜라이더 크기 세팅
    PLAYER->CharacterController()->SetCenter(Vec3(0.f, 1.44f, 0.f));
    PLAYER->CharacterController()->SetRadius(1.2f);
    PLAYER->CharacterController()->SetHeight(2.94f);

    CCapsuleCollider* BodyCol = PLAYER->GetChildObject(L"Body Collider")->CapsuleCollider();
    BodyCol->SetCenter(Vec3(0.f, 1.1f, 0.f));
    BodyCol->SetRadius(1.1f);
    BodyCol->SetHeight(2.9f);

    PLAYERCTRL->SetSpeed(m_Speed);
    PLAYERCTRL->SetRotSpeed(m_Speed);

    // PointLight
    CGameObject* PointLight = PLAYER->GetChildObject(L"DeformLight PointLight");
    if (PointLight)
    {
        m_PointLight = PointLight->GetScript<CKirbyLightScript>();
    }
}
