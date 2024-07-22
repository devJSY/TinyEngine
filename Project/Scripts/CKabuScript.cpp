#include "pch.h"
#include "CKabuScript.h"

CKabuScript::CKabuScript()
    : CMonsterUnitScript(KABUSCRIPT)
    , m_eState(KABU_STATE::Patrol)
    , m_fLerpValue(0.f)
    , m_vDamageDir{}
    , m_bFlag(false)
    , m_vPatrolDir{}
    , m_vOriginPos{}
    , m_vDestPos{}
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vPatrolDir, "PatrolDir");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOriginPos, "OriginPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDestPos, "DestPos");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLerpValue, "LerpValue");
}

CKabuScript::CKabuScript(const CKabuScript& _Origin)
    : CMonsterUnitScript(_Origin)
    , m_eState(KABU_STATE::Patrol)
    , m_fLerpValue(_Origin.m_fLerpValue)
    , m_vDamageDir{}
    , m_bFlag(false)
    , m_vPatrolDir{}
    , m_vOriginPos{}
    , m_vDestPos{}
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vPatrolDir, "PatrolDir");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOriginPos, "OriginPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDestPos, "DestPos");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fLerpValue, "LerpValue");
}

CKabuScript::~CKabuScript()
{
}

void CKabuScript::begin()
{
    ChangeState(KABU_STATE::Patrol);

    m_vOriginPos = Transform()->GetLocalPos();
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
    case KABU_STATE::Eaten: {
        Eaten();
    }
    break;
    case KABU_STATE::End:
        break;
    default:
        break;
    }
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
        Animator()->Play(ANIMPREFIX("Wait"), false);
    }
    break;
    case KABU_STATE::Damage: {
        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case KABU_STATE::Eaten: {
        Animator()->Play(ANIMPREFIX("Damage"));

        m_vDamageDir.Normalize();
        Vec3 vUp = Vec3(0.f, 0.f, -1.f) == m_vDamageDir ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);
        Quat vQuat = Quat::LookRotation(-m_vDamageDir, vUp);
        Transform()->SetWorldRotation(vQuat);
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
    switch (_state)
    {
    case KABU_STATE::Patrol: {
    }
    break;
    case KABU_STATE::Damage: {
        m_bFlag = false;
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

void CKabuScript::Patrol()
{
    Rotating();

    PatrolMove();
}

void CKabuScript::Damage()
{
    if (!m_bFlag)
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        m_vDamageDir.Normalize();
        m_vDamageDir.y = 1.5f;
        Rigidbody()->AddForce(m_vDamageDir * 50.f, ForceMode::Impulse);
        m_bFlag = true;
    }

    if (Animator()->IsFinish())
    {
        ChangeState(KABU_STATE::Patrol);
    }
}

void CKabuScript::Eaten()
{
    Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * 10.f, ForceMode::Force);
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
    Vec3 vPos = Transform()->GetLocalPos();
    float fSpeed = GetCurInfo().Speed;

    // 목적지까지 Patrol이 끝났는데 반복 정찰이라면 다시 원래 자리로 돌아간다.
    if ((m_vDestPos.x - 5.f <= vPos.x && m_vDestPos.x + 5.f >= vPos.x) && (m_vDestPos.z - 5.f <= vPos.z && m_vDestPos.z + 5.f >= vPos.z))
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Vec3 _vTemp = m_vDestPos;
        m_vDestPos = m_vOriginPos;
        m_vOriginPos = _vTemp;
        m_vPatrolDir *= -1.f;
    }

    Vec3 vUP = Vec3(0.f, 0.f, -1.f) == m_vPatrolDir ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    // Rotating시에는 WorldRotation
    Quat _vOriginQuat = Quat::LookRotation(-m_vPatrolDir, vUP);

    Quat _vTargetQuat = Quat::LookRotation(-1.f * (m_vDestPos - vPos), vUP);

    _vTargetQuat = Quat::Slerp(_vOriginQuat, _vTargetQuat, m_fLerpValue * DT);

    m_vPatrolDir = m_vPatrolDir.Normalize();
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

void CKabuScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (KABU_STATE::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();
    bool flag = false;

    UnitHit hit;
    ZeroMemory(&hit, sizeof(hit));
    /**********************
    | 1. Player ATK Hit
    ***********************/

    // 충돌한 오브젝트가 플레이어 공격인지 확인
    if (LAYER_PLAYERATK == pObj->GetLayerIdx())
    {
        flag = true;
        ChangeState(KABU_STATE::Damage);
        m_vDamageDir = pObj->GetParent()->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
    }

    /**********************
    | 2. Player Body Hit
    ***********************/
    // 충돌한 오브젝트가 PlayerBody인지 확인

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(KABU_STATE::Eaten);
            m_vDamageDir = -pObj->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
            return;
        }

        L"Body Collider" == pObj->GetName() ? pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(GetHitInfo()) : void();
    }

    // 둘 중 하나라도 피격 되었다면 체력 확인
    if (flag)
    {
        if (GetCurInfo().HP - hit.Damage <= 0.f)
        {
            ChangeState(KABU_STATE::Death);
        }
    }
}

void CKabuScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이는게 끝난 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(KABU_STATE::Patrol);
        }
    }
}

UINT CKabuScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);
    fwrite(&m_vDestPos, sizeof(Vec3), 1, _File);
    fwrite(&m_fLerpValue, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CKabuScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);
    fread(&m_vDestPos, sizeof(Vec3), 1, _File);
    fread(&m_fLerpValue, sizeof(float), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(float);

    return MemoryByte;
}