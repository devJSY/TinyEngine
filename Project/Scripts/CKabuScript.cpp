#include "pch.h"
#include "CKabuScript.h"

CKabuScript::CKabuScript()
    : CMonsterUnitScript(KABUSCRIPT)
    , m_eState(KABU_STATE::Patrol)
    , m_fLerpValue(0.f)
{
}

CKabuScript::~CKabuScript()
{
}

void CKabuScript::begin()
{
    ChangeState(KABU_STATE::Patrol);
}

void CKabuScript::tick()
{
    switch (m_eState)
    {
    case KABU_STATE::Patrol: {
        Patrol();
    }
    break;
    case KABU_STATE::Damage: {
        Damage();
    }
    break;
    case KABU_STATE::Death: {
        Death();
    }
    break;
    case KABU_STATE::End:
        break;
    default:
        break;
    }
}

void CKabuScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CKabuScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}

void CKabuScript::ChangeState(KABU_STATE _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CKabuScript::EnterState(KABU_STATE _state)
{
    switch (_state)
    {
    case KABU_STATE::Patrol: {
        Animator()->Play(ANIMPREFIX(L"Wait"), false);
    }
    break;
    case KABU_STATE::Damage: {
        Animator()->Play(ANIMPREFIX(L"Damage"), false);
    }
    break;
    case KABU_STATE::Death:
        break;
    case KABU_STATE::End:
        break;
    default:
        break;
    }
}

void CKabuScript::ExitState(KABU_STATE _state)
{
}

void CKabuScript::Patrol()
{
    TransformRotate();
}

void CKabuScript::Damage()
{
    if (Animator()->IsFinish())
    {
        ChangeState(KABU_STATE::Patrol);
    }
}

void CKabuScript::Death()
{
    if (Animator()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CKabuScript::PatrolMove()
{
    Vec3 vUp = {};
    Vec3 vPos = Transform()->GetLocalPos();
    float fSpeed = GetCurInfo().Speed;

    // 목적지까지 Patrol이 끝났는데 반복 정찰이라면 다시 원래 자리로 돌아간다.
    if ((m_vDestPos.x - 1.f <= vPos.x && m_vDestPos.x + 1.f >= vPos.x) && (m_vDestPos.x - 1.f <= vPos.x && m_vDestPos.x + 1.f >= vPos.x) &&
        (m_vDestPos.x - 1.f <= vPos.x && m_vDestPos.x + 1.f >= vPos.x))
    {
        Vec3 _vTemp = m_vDestPos;
        m_vDestPos = m_vOriginPos;
        m_vOriginPos = m_vDestPos;
    }

    if (Vec3(0.f, 0.f, -1.f) == m_vPatrolDir)
    {
        vUp = Vec3(0.f, -1.f, 0.f);
    }

    // Rotating시에는 WorldRotation
    Quat _vOriginQuat = Quat::LookRotation(m_vPatrolDir, vUp);

    Quat _vTargetQuat = Quat::LookRotation(-1.f * (m_vDestPos - vPos), vUp);

    _vTargetQuat = Quat::Slerp(_vOriginQuat, _vTargetQuat, m_fLerpValue * DT);

    Rigidbody()->SetVelocity(m_vPatrolDir * fSpeed * DT);

    m_vPatrolDir = CaculateDir(_vTargetQuat.ToEuler()).Normalize();
}

Vec3 CKabuScript::CaculateDir(Vec3 _vRadian)
{
    Quat _LocalQuat = Quat::CreateFromAxisAngle(Vec3(1.f, 0.f, 0.f), _vRadian.x) * Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), _vRadian.y) *
                      Quat::CreateFromAxisAngle(Vec3(0.f, 0.f, 1.f), _vRadian.z);

    Matrix matRot = Matrix::CreateFromQuaternion(_LocalQuat);
    return XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), matRot);
}