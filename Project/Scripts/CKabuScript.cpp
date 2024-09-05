#include "pch.h"
#include "CKabuScript.h"

#include "CPlayerMgr.h"

CKabuScript::CKabuScript()
    : CMonsterUnitScript(KABUSCRIPT)
    , m_pSmokeSpawner(nullptr)
    , m_eState(KabuState::Patrol)
    , m_vCenterPos{}
    , m_vDamageDir{}
    , m_vDir{}
    , m_bFlag(false)
    , m_bCurved(false)
    , m_bInverse(false)
    , m_bHalfCurved(false)
    , m_bHalfFlag(false)
    , m_fAccTime(0.f)
    , m_fWaitTime(1.5f)
    , m_fAccTime2(0.f)
    , m_vOriginPos{}
    , m_vDestPos{}
    , m_bIsEnter(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPos, "CenterPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOriginPos, "OriginPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDestPos, "DestPos");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCurved, "Curved");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bHalfCurved, "Half Curved");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bInverse, "MoveZ");
}

CKabuScript::CKabuScript(const CKabuScript& Origin)
    : CMonsterUnitScript(Origin)
    , m_pSmokeSpawner(nullptr)
    , m_eState(KabuState::Patrol)
    , m_vCenterPos{}
    , m_vDamageDir{}
    , m_vDir{}
    , m_bFlag(false)
    , m_bCurved(Origin.m_bCurved)
    , m_bInverse(Origin.m_bInverse)
    , m_bHalfCurved(Origin.m_bHalfCurved)
    , m_bHalfFlag(Origin.m_bHalfFlag)
    , m_fWaitTime(1.5f)
    , m_fAccTime2(0.f)
    , m_fAccTime(0.f)
    , m_vOriginPos{}
    , m_vDestPos{}
    , m_bIsEnter(false)
{
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vCenterPos, "CenterPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vOriginPos, "OriginPos");
    AddScriptParam(SCRIPT_PARAM::VEC3, &m_vDestPos, "DestPos");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bCurved, "Curved");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bHalfCurved, "Half Curved");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bInverse, "MoveZ");
}

CKabuScript::~CKabuScript()
{
}

void CKabuScript::begin()
{
    CMonsterUnitScript::begin();

    ChangeState(KabuState::Fall);

    m_vOriginPos = Transform()->GetWorldPos();

    if (m_bInverse)
    {
        m_vPrevDir = m_vDir = Vec3(0.f, 0.f, 1.f);
    }
    else
    {
        m_vPrevDir = m_vDir = Vec3(1.f, 0.f, 0.f);
    }

    m_pSmokeSpawner = GetOwner()->GetChildObject(L"KabuSmokeSpawner");

    SetRayCast(15.f);
}

void CKabuScript::tick()
{
    CMonsterUnitScript::tick();

    CheckDamage();

    FSM();

    if (KabuState::Eaten != m_eState && GetResistState())
    {
        ChangeState(KabuState::Eaten);
    }
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
    UINT Layer = _OtherCollider->GetOwner()->GetLayerIdx();

    if (Layer == LAYER_PLAYER_TRIGGER && L"Body Collider" == pObj->GetName())
    {
        pObj->GetParent()->GetScript<CUnitScript>()->GetDamage(hitInfo);
        BodyAttackSound();
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
    if (m_eState == KabuState::Death)
        return;

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
    SmokeSpawnOn();

    switch (_state)
    {
    case KabuState::Patrol: {
        m_vDir = m_vPrevDir;
        Animator()->Play(ANIMPREFIX("Wait"), false, false, 1.5f);
    }
    break;
    case KabuState::Return: {
        m_vDir = (m_vDestPos - Transform()->GetWorldPos()).Normalize();
        Animator()->Play(ANIMPREFIX("Wait"), false, false, 1.5f);
    }
    break;
    case KabuState::Fall: {
        SmokeSpawnOff();
        Animator()->Play(ANIMPREFIX("Fall"), true, false, 1.5f);
    }
    break;
    case KabuState::Landing: {
        LandingSmokeEffect(Vec3(0.f, -15.f, 0.f));
        SmokeSpawnOff();
        Animator()->Play(ANIMPREFIX("Landing"), false, false, 1.5f);
    }
    break;
    case KabuState::Damage: {
        SmokeSpawnOff();
        SetSparkle(true);

        Vec3 vFollowDir = (PLAYER->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
        vFollowDir.y = 0.f;

        Transform()->SetDirection(vFollowDir);

        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

        Vec3 vHitDir = GetOwner()->GetScript<CUnitScript>()->GetHitDir();
        float fForce = 0.f;
        if (GetCurInfo().HP <= 0.1f)
        {
            fForce = 200.f;
            vHitDir.y = 1.5f;
        }
        else
        {
            fForce = 140.f;
            vHitDir.y = 1.f;
        }

        Rigidbody()->AddForce(vHitDir.Normalize() * fForce, ForceMode::Impulse);

        Animator()->Play(ANIMPREFIX("Damage"), false, false, 1.5f);
    }
    break;
    case KabuState::Eaten: {
        SmokeSpawnOff();
        Animator()->Play(ANIMPREFIX("Damage"), true, false, 1.5f);
    }
    break;
    case KabuState::Death: {
        SmokeSpawnOff();
        SpawnDeadSmokeEffect();
        Animator()->Play(ANIMPREFIX("Damage"), false, false, 1.5f);
    }
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
    case KabuState::Return: {
        Return();
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
        m_vPrevDir = m_vDir;
    }
    break;
    case KabuState::Damage: {
        m_bFlag = false;
    }
    break;
    case KabuState::Eaten: {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
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
        LinearMove(m_bInverse);
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

void CKabuScript::LinearMove(bool _moveZ)
{
    Vec3 vPos = Transform()->GetWorldPos();

    m_vDir.y = 0.f;

    Rigidbody()->SetVelocity(m_vDir * GetCurInfo().Speed * DT + Vec3(0.f, -9.81f, 0.f));

    if (_moveZ)
    {
        if (m_vDir.z <= -0.8f)
        {
            if (m_vOriginPos.z >= vPos.z)
            {
                m_vDir = Vec3(0.f, 0.f, 1.f);
                Rigidbody()->SetVelocity(Vec3(0.f, -9.81f, 0.f));
            }
        }
        else
        {
            if (m_vDestPos.z <= vPos.z)
            {
                m_vDir = Vec3(0.f, 0.f, -1.f);
                Rigidbody()->SetVelocity(Vec3(0.f, -9.81f, 0.f));
            }
        }
    }
    else
    {
        if (m_vDir.x <= -0.8f)
        {
            if (m_vOriginPos.x >= vPos.x)
            {
                m_vDir = Vec3(1.f, 0.f, 0.f);
                Rigidbody()->SetVelocity(Vec3(0.f, -9.81f, 0.f));
            }
        }
        else
        {
            if (m_vDestPos.x <= vPos.x)
            {
                m_vDir = Vec3(-1.f, 0.f, 0.f);
                Rigidbody()->SetVelocity(Vec3(0.f, -9.81f, 0.f));
            }
        }
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
        if (!m_bIsEnter)
        {
            m_fAccTime2 += DT;
            if (m_fAccTime2 >= m_fWaitTime)
            {
                m_bIsEnter = true;
                m_fAccTime2 = 0.f;
            }
        }
        else
        {
            ChangeState(KabuState::Patrol);
        }
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
        if (Animator()->IsFinish())
        {
            ChangeState(KabuState::Fall);
        }
    }
}
#pragma endregion

#pragma region EATEN
void CKabuScript::Eaten()
{
    if (!GetResistState())
    {
        ChangeState(KabuState::Return);
    }
}
#pragma endregion

#pragma region DEATH
void CKabuScript::Death()
{
    if (Animator()->IsFinish())
    {
        SpawnDeadEffect(1);
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}
void CKabuScript::SmokeSpawnOn()
{
    if (nullptr != m_pSmokeSpawner)
        m_pSmokeSpawner->SetActive(true);
}
void CKabuScript::SmokeSpawnOff()
{
    if (nullptr != m_pSmokeSpawner)
        m_pSmokeSpawner->SetActive(false);
}
#pragma endregion

void CKabuScript::Return()
{
    Vec3 vPos = Transform()->GetLocalPos();

    Rigidbody()->SetVelocity(m_vDir * GetCurInfo().Speed * DT);

    if ((m_vDestPos.x - 10.f <= vPos.x && vPos.x <= m_vDestPos.x + 5.f) && (m_vDestPos.z - 5.f <= vPos.z && vPos.z <= m_vDestPos.z + 5.f))
    {
        Rigidbody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
        Vec3 vTemp = m_vDestPos;
        m_vDestPos = m_vOriginPos;
        m_vOriginPos = vTemp;
        m_vDestPos.y = 0.f;
        m_vOriginPos.y = 0.f;
        ChangeState(KabuState::Patrol);
    }
}