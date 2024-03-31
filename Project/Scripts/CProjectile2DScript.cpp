#include "pch.h"
#include "CProjectile2DScript.h"
#include "CPlayerScript.h"
#include "CEnemyScript.h"

CProjectile2DScript::CProjectile2DScript()
    : CScript(PROJECTILE2DSCRIPT)
    , m_pTarget(nullptr)
    , m_vDir(Vec3(0.f, 0.f, 0.f))
    , m_Speed(0.f)
    , m_ATK(5)
    , m_Duration(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::INT, &m_ATK, "ATK");
}

CProjectile2DScript::CProjectile2DScript(const CProjectile2DScript& origin)
    : CScript(origin)
    , m_pTarget(origin.m_pTarget)
    , m_vDir(origin.m_vDir)
    , m_Speed(origin.m_Speed)
    , m_ATK(origin.m_ATK)
    , m_Duration(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDir, "Base Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
    AddScriptParam(SCRIPT_PARAM::INT, &m_ATK, "ATK");
}

CProjectile2DScript::~CProjectile2DScript()
{
}

void CProjectile2DScript::tick()
{
    m_Duration += DT;
    if (m_Duration > 10.f)
        GamePlayStatic::DestroyGameObject(GetOwner());

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

    m_pTarget = _pTarget;

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

void CProjectile2DScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (nullptr == m_pTarget)
        return;

    Vec3 ProjectilePos = Transform()->GetWorldPos();

    if (L"Player" == m_pTarget->GetName())
    {
        // 플레이어 타겟 투사체
        CPlayerScript* pPlayerScript = _OtherCollider->GetOwner()->GetScript<CPlayerScript>();
        if (nullptr == pPlayerScript)
            return;

        Vec3 PlayerPos = _OtherCollider->GetOwner()->Transform()->GetWorldPos();
        ProjectilePos.z = 0.f;
        PlayerPos.z = 0.f;
        pPlayerScript->TakeHit(m_ATK, (PlayerPos - ProjectilePos).Normalize());
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
    else
    {
        // 몬스터 타겟 투사체
        CEnemyScript* pEnemyScript = _OtherCollider->GetOwner()->GetScript<CEnemyScript>();
        if (nullptr == pEnemyScript)
            return;

        Vec3 EnemyPos = _OtherCollider->GetOwner()->Transform()->GetWorldPos();
        ProjectilePos.z = 0.f;
        EnemyPos.z = 0.f;
        pEnemyScript->TakeHit(m_ATK, (EnemyPos - ProjectilePos).Normalize());
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CProjectile2DScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_vDir, sizeof(Vec3), 1, _File);
    fwrite(&m_Speed, sizeof(float), 1, _File);
    fwrite(&m_ATK, sizeof(int), 1, _File);
}

void CProjectile2DScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_vDir, sizeof(Vec3), 1, _File);
    fread(&m_Speed, sizeof(float), 1, _File);
    fread(&m_ATK, sizeof(int), 1, _File);
}
