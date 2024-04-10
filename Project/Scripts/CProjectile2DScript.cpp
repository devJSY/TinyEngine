#include "pch.h"
#include "CProjectile2DScript.h"
#include "CPlayerScript.h"
#include "CEnemyScript.h"

CProjectile2DScript::CProjectile2DScript()
    : CScript(PROJECTILE2DSCRIPT)
    , m_pTarget(nullptr)
    , m_vBaseDir(Vec3(0.f, 0.f, 0.f))
    , m_vInitDir(Vec3(0.f, 0.f, 0.f))
    , m_Force(0.f)
    , m_ATK(5)
    , m_Duration(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vBaseDir, "Base Direction");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vInitDir, "Initial Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Force, "Force");
    AddScriptParam(SCRIPT_PARAM::INT, &m_ATK, "ATK");
}

CProjectile2DScript::CProjectile2DScript(const CProjectile2DScript& origin)
    : CScript(origin)
    , m_pTarget(origin.m_pTarget)
    , m_vBaseDir(origin.m_vBaseDir)
    , m_vInitDir(origin.m_vInitDir)
    , m_Force(origin.m_Force)
    , m_ATK(origin.m_ATK)
    , m_Duration(0.f)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vBaseDir, "Base Direction");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vInitDir, "Initial Direction");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Force, "Force");
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

    if (nullptr == m_pTarget)
    {
        m_vBaseDir.Normalize();
        Rigidbody2D()->AddForce(Vec2(m_vBaseDir.x, m_vBaseDir.y) * m_Force);
    }
    else
    {
        if (m_Duration < 0.3f)
        {
            m_vInitDir.Normalize();
            Rigidbody2D()->AddForce(Vec2(m_vInitDir.x, m_vInitDir.y) * m_Force);
        }
        else
        {
            // 타겟 위치 추적
            Vec3 ProjPos = Transform()->GetRelativePos();
            Vec3 TargetPos = m_pTarget->Transform()->GetRelativePos();
            ProjPos.z = 0.f;
            TargetPos.z = 0.f;
            Vec3 vToTargetDir = TargetPos - ProjPos;

            vToTargetDir.Normalize();
            Rigidbody2D()->AddForce(Vec2(vToTargetDir.x, vToTargetDir.y) * m_Force);
        }

        // 현재 진행방향에 맞춰서 Z축 회전
        Vec2 vel = Rigidbody2D()->GetVelocity();
        ZAxisAlign(Vec3(vel.x, vel.y, 0.f), m_vBaseDir);
    }
}

void CProjectile2DScript::SetTarget(CGameObject* _pTarget, Vec3 _vBaseDir)
{
    if (nullptr == _pTarget)
        return;

    m_pTarget = _pTarget;
    m_vBaseDir = _vBaseDir;

    Vec3 ProjPos = Transform()->GetRelativePos();
    Vec3 TargetPos = _pTarget->Transform()->GetRelativePos();

    ProjPos.z = 0.f;
    TargetPos.z = 0.f;

    Vec3 vToTargetDir = TargetPos - ProjPos;
    vToTargetDir.Normalize();

    ZAxisAlign(vToTargetDir, m_vBaseDir);
}

void CProjectile2DScript::ZAxisAlign(Vec3 _vToDir, Vec3 _vBaseDir)
{
    _vToDir.z = 0.f;
    _vToDir.Normalize();

    _vBaseDir.z = 0.f;
    _vBaseDir.Normalize();

    float tTheta = _vBaseDir.Dot(_vToDir);

    Vec3 rot = Transform()->GetRelativeRotation();

    // 180도보다 큰경우 y방향 반전
    if (tTheta < 0.f)
    {
        rot.y = XM_PI;
        _vBaseDir = -_vBaseDir;
    }
    else
    {
        rot.y = 0.f;
    }

    rot.z = cosf(tTheta);

    Vec3 Cross = _vBaseDir.Cross(_vToDir);
    if (Cross.z < 0.f)
        rot.z *= -1.f;

    Transform()->SetRelativeRotation(rot);
}

void CProjectile2DScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (_OtherCollider->GetOwner() == m_pTarget)
    {
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
    else
    {
        // Ground 레이어와 충돌시 삭제
        if (L"Ground" == CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(_OtherCollider->GetOwner()->GetLayerIdx())->GetName())
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
    }
}

void CProjectile2DScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_vBaseDir, sizeof(Vec3), 1, _File);
    fwrite(&m_vInitDir, sizeof(Vec3), 1, _File);
    fwrite(&m_Force, sizeof(float), 1, _File);
    fwrite(&m_ATK, sizeof(int), 1, _File);
}

void CProjectile2DScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_vBaseDir, sizeof(Vec3), 1, _File);
    fread(&m_vInitDir, sizeof(Vec3), 1, _File);
    fread(&m_Force, sizeof(float), 1, _File);
    fread(&m_ATK, sizeof(int), 1, _File);
}
