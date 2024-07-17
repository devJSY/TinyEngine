#include "pch.h"
#include "CMonsterUnitScript.h"

CMonsterUnitScript::CMonsterUnitScript(UINT _Type)
    : CUnitScript(_Type)
    , m_pTargetObj(nullptr)
    , m_HitInfo{}
{
    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP current");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
    AddScriptParam(SCRIPT_PARAM::INT, &m_HitInfo.Type, "DamageType");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Damage, "Damage");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Duration, "DamageDuration");
}

CMonsterUnitScript::CMonsterUnitScript(const CMonsterUnitScript& _Origin)
    : CUnitScript(_Origin)
    , m_HitInfo(_Origin.m_HitInfo)
{
    AddScriptParam(SCRIPT_PARAM::OBJECT, m_pTargetObj, "Target Object");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.HP, "HP current");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.MAXHP, "HP max");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.RotationSpeed, "Rotation Speed");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_CurInfo.JumpPower, "Jump Power");
    AddScriptParam(SCRIPT_PARAM::INT, &m_HitInfo.Type, "DamageType");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Damage, "Damage");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_HitInfo.Duration, "DamageDuration");
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

        Rigidbody()->SetVelocity(Dir * m_CurInfo.Speed);
    }
    // 타겟 방향으로 이동
    else
    {
        Vec3 ToTargetDir = m_pTargetObj->Transform()->GetWorldPos() - Transform()->GetWorldPos();
        ToTargetDir.Normalize();

        Rigidbody()->SetVelocity(ToTargetDir * m_CurInfo.Speed);
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
    fwrite(&m_HitInfo, sizeof(m_HitInfo), 1, _File);
}

void CMonsterUnitScript::LoadFromLevelFile(FILE* _File)
{
    CUnitScript::LoadFromLevelFile(_File);
    fread(&m_HitInfo, sizeof(m_HitInfo), 1, _File);
}
