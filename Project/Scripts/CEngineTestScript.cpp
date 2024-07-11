#include "pch.h"
#include "CEngineTestScript.h"
#include <Engine\\CTransform.h>

CEngineTestScript::CEngineTestScript()
    : CScript(ENGINETESTSCRIPT)
{
}

CEngineTestScript::~CEngineTestScript()
{
}

void CEngineTestScript::begin()
{
}

void CEngineTestScript::tick()
{
    GamePlayStatic::DrawDebugLine(GetOwner()->Transform()->GetWorldPos(), GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT), 100.f,
                                  Vec3(1.f, 1.f, 0.f), true);

    //// =============================
    //// 1. 타겟 방향으로 회전 설정 예제
    //// =============================
    //{
    //    CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
    //    if (pTarget)
    //    {
    //        Vec3 Dir = Transform()->GetWorldPos() - pTarget->Transform()->GetWorldPos();
    //        Dir.Normalize();
    //        Quat Quaternion = Quat::LookRotation(Dir, Vec3(0.f, 1.f, 0.f));
    //        Transform()->SetWorldRotation(Quaternion);
    //    }
    //}

    //// =============================
    //// 2. 타겟과의 각도
    //// =============================
    //{
    //    CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
    //    if (pTarget)
    //    {
    //        Quat Quaternion = Transform()->GetWorldQuaternion();
    //        Quat TargetQuaternion = pTarget->Transform()->GetWorldQuaternion();

    //        float Angle = Quat::Angle(Quaternion, TargetQuaternion);
    //        Angle = XMConvertToDegrees(Angle);

    //        LOG(Log, "%f", Angle);
    //    }
    //}

    //// =============================
    //// 3. 특정 축 기준 회전
    //// =============================
    //{
    //    CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
    //    if (pTarget)
    //    {
    //        static float Angle = 0.f;
    //        Angle += DT;
    //        Quat Quaternion = Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), Angle);
    //        Transform()->SetWorldRotation(Quaternion);
    //    }
    //}

    //// =============================
    //// 4. 특정 축으로 설정하는 예제
    //// =============================
    //{
    //    if (KEY_TAP(KEY::R))
    //    {
    //        CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
    //        if (pTarget)
    //        {
    //            Quat Quaternion = Quat::FromToRotation(Vec3(0.f, 1.f, 0.f), Transform()->GetWorldDir(DIR_TYPE::FRONT));
    //            Transform()->SetWorldRotation(Quaternion);
    //        }
    //    }
    //}

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

    //// =============================
    //// 6. RotateToWard 예제
    //// 타겟 오브젝트의 회전 값과 동일해지게 보간하는 예제
    //// =============================
    //{
    //    CGameObject* pTarget = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Target");
    //    if (pTarget)
    //    {
    //        Quat Quaternion = Transform()->GetWorldQuaternion();
    //        Quat TargetQuaternion = pTarget->Transform()->GetWorldQuaternion();

    //        float RotSpeed = 1.f;

    //        Quaternion.RotateTowards(TargetQuaternion, DT * RotSpeed);

    //        Transform()->SetWorldRotation(Quaternion);
    //    }
    //}
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
