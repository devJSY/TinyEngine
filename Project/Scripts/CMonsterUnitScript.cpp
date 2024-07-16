#include "pch.h"
#include "CMonsterUnitScript.h"

CMonsterUnitScript::CMonsterUnitScript(UINT _Type)
    : CUnitScript(_Type)
    , m_pTargetObj(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP current");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
}

CMonsterUnitScript::CMonsterUnitScript(const CMonsterUnitScript& _Origin)
    : CUnitScript(_Origin)
    , m_pTargetObj(_Origin.m_pTargetObj)
{
    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP current");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
}

CMonsterUnitScript::~CMonsterUnitScript()
{
}

void CMonsterUnitScript::RigidbodyMove(CGameObject* _pTargetObj)
{
    // 전방 방향으로 이동
    if (nullptr == _pTargetObj)
    {
        Vec3 Dir = Transform()->GetWorldDir(DIR_TYPE::FRONT);

        Rigidbody()->AddForce(Dir * m_CurInfo.Speed, ForceMode::VelocityChange);
        // Rigidbody()->SetVelocity(FrontDir * m_CurInfo.Speed); 동일한 기능을 하는 코드
    }
    // 타겟 방향으로 이동
    else
    {
        Vec3 ToTargetDir = m_pTargetObj->Transform()->GetWorldPos() - Transform()->GetWorldPos();
        ToTargetDir.Normalize();

        Rigidbody()->AddForce(ToTargetDir * m_CurInfo.Speed, ForceMode::VelocityChange);
        // Rigidbody()->SetVelocity(FrontDir * m_CurInfo.Speed); 동일한 기능을 하는 코드
    }
}

void CMonsterUnitScript::TransformRotate()
{
    if (nullptr == m_pTargetObj)
        return;

    Vec3 Dir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 ToTargetDir = Transform()->GetWorldPos() - m_pTargetObj->Transform()->GetWorldPos();
    ToTargetDir.y = 0.f; // Y축 고정
    ToTargetDir.Normalize();

    // 아주 미세한 값인 경우 보간 X
    if (ToTargetDir.Dot(Dir) < cosf(0.f) - 0.0000001f)
    {
        // 예외처리 Dir 이 Vec3(0.f, 0.f, -1.f)인경우 Up벡터가 반전됨
        Vec3 up = Vec3(0.f, 1.f, 0.f);
        if (Dir == Vec3(0.f, 0.f, -1.f))
        {
            up = Vec3(0.f, -1.f, 0.f);
        }

        Quat ToWardQuaternion = Quat::LookRotation(ToTargetDir, up);
        Quat SlerpQuat = Quat::Slerp(Transform()->GetWorldQuaternion(), ToWardQuaternion, DT * m_CurInfo.RotationSpeed);
        Transform()->SetWorldRotation(SlerpQuat);
    }
}

void CMonsterUnitScript::SaveToLevelFile(FILE* _File)
{
    CUnitScript::SaveToLevelFile(_File);
}

void CMonsterUnitScript::LoadFromLevelFile(FILE* _File)
{
    CUnitScript::LoadFromLevelFile(_File);
}
