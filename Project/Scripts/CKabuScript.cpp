#include "pch.h"
#include "CKabuScript.h"

#include "CPlayerMgr.h"

CKabuScript::CKabuScript()
    : CMonsterUnitScript(KABUSCRIPT)
    , m_eState(KabuState::Patrol)
    , m_vCenterPos{}
    , m_vDamageDir{}
    , m_bFlag(false)
    , m_bCurved(false)
    , m_bInverse(false)
    , m_bHalfCurved(false)
    , m_bHalfFlag(false)
    , m_fAccTime(0.f)
    , m_vOriginPos{}
    , m_vDestPos{}
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPos, "CenterPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOriginPos, "OriginPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDestPos, "DestPos");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCurved, "Curved");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bHalfCurved, "Half Curved");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bInverse, "Inverse");
}

CKabuScript::CKabuScript(const CKabuScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_eState(KabuState::Patrol)
    , m_vCenterPos{}
    , m_vDamageDir{}
    , m_bFlag(false)
    , m_bCurved(Origin.m_bCurved)
    , m_bInverse(Origin.m_bInverse)
    , m_bHalfCurved(Origin.m_bHalfCurved)
    , m_bHalfFlag(Origin.m_bHalfFlag)
    , m_fAccTime(0.f)
    , m_vOriginPos{}
    , m_vDestPos{}
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPos, "CenterPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOriginPos, "OriginPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDestPos, "DestPos");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCurved, "Curved");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bHalfCurved, "Half Curved");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bInverse, "Inverse");
}

CKabuScript::~CKabuScript()
{
}

void CKabuScript::begin()
{
    CUnitScript::begin();

    ChangeState(KabuState::Patrol);

    m_vOriginPos = Transform()->GetWorldPos();
}

void CKabuScript::tick()
{
    CMonsterUnitScript::tick();

    CheckDamage();

    FSM();
}

void CKabuScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (KabuState::Eaten == m_eState)
        return;

    CGameObject* pObj = _OtherCollider->GetOwner();

    /**********************
    | 2. Player Body Hit
    ***********************/
    // 충돌한 오브젝트가 PlayerBody인지 확인

    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이고 있는 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(KabuState::Eaten);
            m_vDamageDir = -pObj->GetComponent<CTransform>()->GetWorldDir(DIR_TYPE::FRONT);
            return;
        }
    }

    
    Vec3 vDir = PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos();
    UnitHit hitInfo = {DAMAGE_TYPE::NORMAL, vDir.Normalize(), GetCurInfo().ATK, 0.f, 0.f};
    L"Body Collider" == pObj->GetName() ? pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo) : void();
}

void CKabuScript::OnTriggerExit(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (LAYER_PLAYER == pObj->GetLayerIdx())
    {
        // 충돌한 오브젝트 Vaccum 이라면 Collider가 켜진 상태임 즉, 빨아들이는게 끝난 상태
        if (L"Vacuum Collider" == pObj->GetName())
        {
            ChangeState(KabuState::Patrol);
        }
    }
}

UINT CKabuScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::SaveToLevelFile(_File);
    fwrite(&m_vCenterPos, sizeof(Vec3), 1, _File);
    fwrite(&m_vDestPos, sizeof(Vec3), 1, _File);
    fwrite(&m_bCurved, sizeof(bool), 1, _File);
    fwrite(&m_bHalfCurved, sizeof(bool), 1, _File);
    fwrite(&m_bInverse, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CKabuScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CMonsterUnitScript::LoadFromLevelFile(_File);
    fread(&m_vCenterPos, sizeof(Vec3), 1, _File);
    fread(&m_vDestPos, sizeof(Vec3), 1, _File);
    fread(&m_bCurved, sizeof(bool), 1, _File);
    fread(&m_bHalfCurved, sizeof(bool), 1, _File);
    fread(&m_bInverse, sizeof(bool), 1, _File);

    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(Vec3);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

/////////////////////////// FUNC //////////////////////////////////
// 1. EnterState
// 2. FSM
// 3. ExitState
// 4. ChangeState
// 5. CheckDamage
// 6. PatrolMove
// 7. CircleMove
// 8. LinearMove

void CKabuScript::ChangeState(KabuState _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CKabuScript::CheckDamage()
{
    if (IsGetDamage())
    {
        ChangeState(KabuState::Damage);
    }
}

void CKabuScript::EnterState(KabuState _state)
{
    switch (_state)
    {
    case KabuState::Patrol: {
        Animator()->Play(ANIMPREFIX("Wait"), false);
    }
    break;
    case KabuState::Fall: {
        Animator()->Play(ANIMPREFIX("Fall"));
    }
    break;
    case KabuState::Landing: {
        Animator()->Play(ANIMPREFIX("Landing"), false);
    }
    break;
    case KabuState::Damage: {
        SetSparkle(true);

        Transform()->SetDirection((PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize());

        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        Vec3 vHitDir = GetOwner()->GetScript<CUnitScript>()->GetHitDir();
        vHitDir.y = 1.5f;

        Rigidbody()->AddForce(vHitDir.Normalize() * 5.f, ForceMode::Impulse);

        Animator()->Play(ANIMPREFIX("Damage"), false);
    }
    break;
    case KabuState::Eaten: {
        Animator()->Play(ANIMPREFIX("Damage"));

        m_vDamageDir.Normalize();
        Vec3 vUp = Vec3(0.f, 0.f, -1.f) == m_vDamageDir ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);
        Quat vQuat = Quat::LookRotation(-m_vDamageDir, vUp);
        Transform()->SetWorldRotation(vQuat);
    }
    break;
    case KabuState::Death:
        break;
    case KabuState::End:
        break;
    default:
        break;
    }
}

void CKabuScript::FSM()
{
    switch (m_eState)
    {
    case KabuState::Patrol: {
        Patrol();
    }
    break;
    case KabuState::Fall: {
        Fall();
    }
    break;
    case KabuState::Landing: {
        Landing();
    }
    break;
    case KabuState::Damage: {
        Damage();
    }
    break;
    case KabuState::Death: {
        Death();
    }
    break;
    case KabuState::Eaten: {
        Eaten();
    }
    break;
    case KabuState::End:
        break;
    default:
        break;
    }
}

void CKabuScript::ExitState(KabuState _state)
{
    switch (_state)
    {
    case KabuState::Patrol: {
    }
    break;
    case KabuState::Damage: {
        m_bFlag = false;
    }
    break;
    case KabuState::Death:
        break;
    case KabuState::End:
        break;
    default:
        break;
    }
}

void CKabuScript::PatrolMove()
{
    if (m_bCurved)
    {
        CircleMove();
    }
    else
    {
        LinearMove();
    }
}

void CKabuScript::CircleMove()
{
    Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Vec3 vPos = Transform()->GetWorldPos();
    Vec3 vUp = Vec3(0.f, 0.f, -1.f) == vFront ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);

    Vec3 vTemp = (m_vCenterPos - vPos).Normalize();

    Vec3 vPatrolDir = vTemp.Cross(vUp);

    vPatrolDir = vPatrolDir.Normalize();

    if (m_bInverse)
        vPatrolDir *= -1.f;

    Rigidbody()->SetVelocity(vPatrolDir * GetCurInfo().Speed * DT);

    // 현재 위치와 도착 위치 각도 체크
    if (m_bHalfCurved)
    {
        Vec3 vBaseVec = m_vOriginPos - m_vCenterPos;
        Vec3 vDestVec = vPos - m_vCenterPos;

        vBaseVec.y = 0.f;
        vDestVec.y = 0.f;

        Vec3 vBaseUp = vBaseVec == Vec3(0.f, 0.f, -1.f) ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);
        Vec3 vDestUp = vDestVec == Vec3(0.f, 0.f, -1.f) ? Vec3(0.f, -1.f, 0.f) : Vec3(0.f, 1.f, 0.f);

        Quat vBaseQuat = Quat::LookRotation(-vBaseVec, vBaseUp);
        Quat vDestQuat = Quat::LookRotation(-vDestVec, vDestUp);

        float fBaseToDestRadian = Quat::Angle(vBaseQuat, vDestQuat);

        if (!m_bHalfFlag)
        {
            m_fAccTime += DT;
            if (m_fAccTime >= 2.f)
            {
                m_bHalfFlag = true;
            }
        }

        if (fBaseToDestRadian >= XM_PI && m_bHalfFlag)
        {
            m_vDestPos = m_vOriginPos;
            m_vOriginPos = vPos;
            m_bInverse = !m_bInverse;
            m_bHalfFlag = false;
            m_fAccTime = 0.f;
        }
    }
}

void CKabuScript::LinearMove()
{
    Vec3 vPos = Transform()->GetWorldPos();
    Vec3 vUP = Transform()->GetWorldDir(DIR_TYPE::UP);

    Vec3 vDir = m_vDestPos - vPos;
    vDir.Normalize();
    vDir.y = 0;
    Rigidbody()->SetVelocity(vDir * GetCurInfo().Speed * DT);

    if ((m_vDestPos.x - 5.f <= vPos.x && vPos.x <= m_vDestPos.x + 5.f) && (m_vDestPos.z - 5.f <= vPos.z && vPos.z <= m_vDestPos.z + 5.f))
    {
        Vec3 vTemp = m_vDestPos;
        m_vDestPos = m_vOriginPos;
        m_vOriginPos = vTemp;
    }
}

///////////////////////////// FIND FSM ///////////////////////////////////////
// -> FSM STATES ->
// 1. Patrol
// 2. Fall
// 3. Landing
// 4. Damage
// 5. Eaten
// 6. Death

#pragma region PATROL
void CKabuScript::Patrol()
{
    Rotating();

    PatrolMove();
}
#pragma endregion

#pragma region FALL
void CKabuScript::Fall()
{
    if (IsGround())
    {
        ChangeState(KabuState::Landing);
    }
}
#pragma endregion

#pragma region LANDING
void CKabuScript::Landing()
{
    if (Animator()->IsFinish())
    {
        ChangeState(KabuState::Patrol);
    }
}
#pragma endregion

#pragma region DAMAGE
void CKabuScript::Damage()
{
    if (GetCurInfo().HP <= 0.f)
    {
        ChangeState(KabuState::Death);
    }
    else
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
            ChangeState(KabuState::Patrol);
        }
    }
}
#pragma endregion

#pragma region EATEN
void CKabuScript::Eaten()
{
    Rigidbody()->AddForce(Transform()->GetWorldDir(DIR_TYPE::FRONT) * 10.f, ForceMode::Force);
}
#pragma endregion

#pragma region DEATH
void CKabuScript::Death()
{
    if (Animator()->IsFinish())
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}
#pragma endregion