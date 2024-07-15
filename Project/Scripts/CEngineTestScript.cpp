#include "pch.h"
#include "CEngineTestScript.h"
#include <Engine\\CTransform.h>
#include "CKirbyBulletScript.h"

CEngineTestScript::CEngineTestScript()
    : CScript(ENGINETESTSCRIPT)
    , m_Power(1.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Power, "Power");
}

CEngineTestScript::CEngineTestScript(const CEngineTestScript& origin)
    : CScript(origin)
    , m_Power(origin.m_Power)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Power, "Power");
}

CEngineTestScript::~CEngineTestScript()
{
}

void CEngineTestScript::begin()
{
}

void CEngineTestScript::tick()
{
    if (KEY_TAP(KEY::K))
    {
        GetOwner()->SetActive(!GetOwner()->IsActive());
    }

    // CharacterControllerTest();
    // AnimatorTest();
    //  QuaternionExample();

    //// Bullet Test
    // if (KEY_TAP(KEY::N))
    //{
    //     Ptr<CPrefab> bulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Bullet.pref", L"prefab\\Bullet.pref");
    //     if (nullptr != bulletPref)
    //     {
    //         CGameObject* pBullet = bulletPref->Instantiate();
    //         pBullet->Transform()->SetLocalPos(Transform()->GetWorldPos());
    //         CKirbyBulletScript* bulletScript = pBullet->GetScript<CKirbyBulletScript>();
    //         bulletScript->SetInitVelocity(GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 100.f);
    //         GamePlayStatic::SpawnGameObject(pBullet, 0);
    //     }
    // }
}

void CEngineTestScript::CharacterControllerTest()
{
    if (!CharacterController())
        return;

    static float Speed = 10.f;
    static float RotSpeed = 10.f;
    static float RayCastDist = 30.f;
    static float JumpPower = 1.f;
    static Vec3 MoveVelocity = Vec3();

    bool bGrounded = CharacterController()->IsGrounded();
    float GravityVelue = CPhysicsMgr::GetInst()->GetGravity().y;

    // CharacterController의 Grounded 는 마지막 Move()를 기준으로 저장되기 때문에 정확도가 낮음
    // Raycast로 한번 더 땅에 닿은 상태인지 확인한다.
    // if (!bGrounded)
    // {
    Vec3 RayPos = Transform()->GetWorldPos();

    RayPos.y -= (CharacterController()->GetHeight() / 2.f) * CPhysicsMgr::GetInst()->GetPPM();
    RayPos += CharacterController()->GetCenter();

    GamePlayStatic::DrawDebugLine(RayPos, Vec3(0.f, -1.f, 0.f), RayCastDist, Vec3(1.f, 0.f, 1.f), false);
    RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(RayPos, Vec3(0.f, -1.f, 0.f), RayCastDist);
    // bGrounded = nullptr != Hit.pCollisionObj;

    Vec3 FootPos = CharacterController()->GetFootPos();
    int a = 0;
    // }

    // 땅에 닿은 상태면 Velocity Y값 초기화
    if (bGrounded && MoveVelocity.y < 0)
    {
        MoveVelocity.y = 0.f;
    }

    // ↑ ↓
    float Vertical = 0.f;
    if (KEY_TAP(KEY::UP) || KEY_PRESSED(KEY::UP))
    {
        Vertical += 1.f;
    }
    if (KEY_TAP(KEY::DOWN) || KEY_PRESSED(KEY::DOWN))
    {
        Vertical -= 1.f;
    }

    // ← →
    float Horizontal = 0.f;
    if (KEY_TAP(KEY::RIGHT) || KEY_PRESSED(KEY::RIGHT))
    {
        Horizontal += 1.f;
    }
    if (KEY_TAP(KEY::LEFT) || KEY_PRESSED(KEY::LEFT))
    {
        Horizontal -= 1.f;
    }

    // 이동
    Vec3 Dir = Vec3(Horizontal, 0, Vertical);
    Dir.Normalize();
    CharacterController()->Move(Dir * Speed * DT);

    // 방향 전환
    if (Dir.Length() > 0.f)
    {
        Quat ToWardQuaternion = Quat::LookRotation(-Dir, Vec3(0.f, 1.f, 0.f));
        Quat SlerpQuat = Quat::Slerp(Transform()->GetWorldQuaternion(), ToWardQuaternion, DT * RotSpeed);
        Transform()->SetWorldRotation(SlerpQuat);
    }

    // 점프
    if (KEY_TAP(KEY::SPACE) && bGrounded)
    {
        MoveVelocity.y += std::sqrt(JumpPower * -3.f * GravityVelue);
    }

    // 중력 적용
    MoveVelocity.y += GravityVelue * DT;
    CharacterController()->Move(MoveVelocity * DT);
}

void CEngineTestScript::AnimatorTest()
{
    if (!Animator())
        return;

    if (KEY_TAP(KEY::NUM0))
    {
        Animator()->Play(L"Armature|Copy", true, false, 2.5f, 0.f);
    }

    if (KEY_TAP(KEY::NUM1))
    {
        Animator()->Play(L"Armature|Copy", true, false, 2.5f, 0.1f);
    }

    if (KEY_TAP(KEY::NUM2))
    {
        Animator()->Play(L"Armature|Copy", true, false, 2.5f, 1.0f);
    }
}

void CEngineTestScript::QuaternionExample()
{
    // 정면 방향 디버그 렌더
    GamePlayStatic::DrawDebugLine(GetOwner()->Transform()->GetWorldPos(), GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT), 100.f,
                                  Vec3(1.f, 1.f, 0.f), true);

    // =============================
    // 1. 타겟 방향으로 회전 설정 예제
    // =============================
    {
        CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
        if (pTarget)
        {
            Vec3 Dir = Transform()->GetWorldPos() - pTarget->Transform()->GetWorldPos();
            Dir.Normalize();
            Quat Quaternion = Quat::LookRotation(Dir, Vec3(0.f, 1.f, 0.f));
            Transform()->SetWorldRotation(Quaternion);
        }
    }

    // =============================
    // 2. 타겟과의 각도
    // =============================
    {
        CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
        if (pTarget)
        {
            Quat Quaternion = Transform()->GetWorldQuaternion();
            Quat TargetQuaternion = pTarget->Transform()->GetWorldQuaternion();

            float Angle = Quat::Angle(Quaternion, TargetQuaternion);
            Angle = XMConvertToDegrees(Angle);

            LOG(Log, "%f", Angle);
        }
    }

    // =============================
    // 3. 특정 축 기준 회전
    // =============================
    {
        CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
        if (pTarget)
        {
            static float Angle = 0.f;
            Angle += DT;
            Quat Quaternion = Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), Angle);
            Transform()->SetWorldRotation(Quaternion);
        }
    }

    // =============================
    // 4. 특정 축으로 설정하는 예제
    // =============================
    {
        if (KEY_TAP(KEY::R))
        {
            CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
            if (pTarget)
            {
                Quat Quaternion = Quat::FromToRotation(Vec3(0.f, 1.f, 0.f), Transform()->GetWorldDir(DIR_TYPE::FRONT));
                Transform()->SetWorldRotation(Quaternion);
            }
        }
    }

    // =============================
    // 5. Slerp 예제
    // =============================
    {
        CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
        if (pTarget)
        {
            Quat Quaternion = Transform()->GetWorldQuaternion();

            Vec3 Dir = Transform()->GetWorldPos() - pTarget->Transform()->GetWorldPos();
            Dir.Normalize();
            Quat ToTargetQuaternion = Quat::LookRotation(Dir, Vec3(0.f, 1.f, 0.f));

            float RotSpeed = 2.f;

            Quat SlerpQuat = Quat::Slerp(Quaternion, ToTargetQuaternion, DT * RotSpeed);

            // 보간 이기 때문에 무한히 가까워지지만 완전히 동일해지지않음
            // 어느정도 가까워진 경우 예외처리 필요
            Transform()->SetWorldRotation(SlerpQuat);
        }
    }

    // =============================
    // 6. RotateToWard 예제
    // 타겟 오브젝트의 회전 값과 동일해지게 보간하는 예제
    // =============================
    {
        CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
        if (pTarget)
        {
            Quat Quaternion = Transform()->GetWorldQuaternion();
            Quat TargetQuaternion = pTarget->Transform()->GetWorldQuaternion();

            float RotSpeed = 1.f;

            Quaternion.RotateTowards(TargetQuaternion, DT * RotSpeed);

            Transform()->SetWorldRotation(Quaternion);
        }
    }
}

void CEngineTestScript::OnCollisionEnter(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnCollisionStay(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnCollisionExit(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnTriggerEnter(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnTriggerStay(CCollider* _OtherCollider)
{
}

void CEngineTestScript::OnTriggerExit(CCollider* _OtherCollider)
{
}

void CEngineTestScript::SaveToLevelFile(FILE* _File)
{
}

void CEngineTestScript::LoadFromLevelFile(FILE* _File)
{
}
