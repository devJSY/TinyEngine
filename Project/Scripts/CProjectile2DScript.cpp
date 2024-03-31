#include "pch.h"
#include "CProjectile2DScript.h"

CProjectile2DScript::CProjectile2DScript()
    : CScript(PROJECTILE2DSCRIPT)
    , m_vDir(Vec3(0.f, 0.f, 0.f))
    , m_Speed(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
}

CProjectile2DScript::CProjectile2DScript(const CProjectile2DScript& origin)
    : CScript(origin)
    , m_vDir(origin.m_vDir)
    , m_Speed(origin.m_Speed)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Base Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
}

CProjectile2DScript::~CProjectile2DScript()
{
}

void CProjectile2DScript::tick()
{
    if (nullptr == Rigidbody2D())
        return;

    m_vDir.Normalize();
    m_vDir *= m_Speed;
    Rigidbody2D()->SetVelocity(Vec2(m_vDir.x, m_vDir.y));
}

void CProjectile2DScript::SetTarget(CGameObject* _pTarget, Vec3 _vBaseDir)
{
    if (nullptr == _pTarget)
        return;

    Vec3 ProjPos = Transform()->GetRelativePos();
    Vec3 TargetPos = _pTarget->Transform()->GetRelativePos();

    ProjPos.z = 0.f;
    TargetPos.z = 0.f;

    m_vDir = TargetPos - ProjPos;
    m_vDir.Normalize();

    _vBaseDir.z = 0.f;
    _vBaseDir.Normalize();

    float tTheta = _vBaseDir.Dot(m_vDir);

    Vec3 rot = Transform()->GetRelativeRotation();

    // 180도보다 큰경우 y방향 반전
    if (tTheta < 0.f)
    {
        rot.y = XM_PI;
        _vBaseDir = -_vBaseDir;
    }

    rot.z = cosf(tTheta);

    Vec3 Cross = _vBaseDir.Cross(m_vDir);
    if (Cross.z < 0.f)
        rot.z *= -1.f;

    Transform()->SetRelativeRotation(rot);
}

void CProjectile2DScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_vDir, sizeof(Vec3), 1, _File);
    fwrite(&m_Speed, sizeof(float), 1, _File);
}

void CProjectile2DScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_vDir, sizeof(Vec3), 1, _File);
    fread(&m_Speed, sizeof(float), 1, _File);
}
