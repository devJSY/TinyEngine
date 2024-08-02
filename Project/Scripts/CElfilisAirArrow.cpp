#include "pch.h"
#include "CElfilisAirArrow.h"

CElfilisAirArrow::CElfilisAirArrow()
    : CScript(ELFILISAIRARROW)
    , m_Target(nullptr)
    , m_Type(ArrowType::UP)
    , m_ArrowIdx(0)
    , m_Step(0)
    , m_AccTime(0.f)
    , m_bGround(false)
{
}

CElfilisAirArrow::~CElfilisAirArrow()
{
}

void CElfilisAirArrow::begin()
{
    GetOwner()->SetActive(false);
}

void CElfilisAirArrow::tick()
{
    m_AccTime += DT;

    switch (m_Step)
    {
    case 0:
        break;
    case 1:
        Spawn();
        break;
    case 2:
        Ready();
        break;
    case 3:
        Wait();
        break;
    case 4:
        Aim();
        break;
    case 5:
        Attack();
        break;
    }
}

void CElfilisAirArrow::StartSpawn()
{
    m_Step = 1;
    m_AccTime = 0.f;

    GetOwner()->SetActive(true);
    Rigidbody()->SetKinematic(true);

    // Position
    Vec3 NewPos = m_InitPos;
    int ReflectIdx = fabs(m_ArrowIdx - 3);
    NewPos += 40.f * (-3 + m_ArrowIdx) * -m_InitDir[(UINT)DIR_TYPE::RIGHT];
    NewPos += -35.f * ReflectIdx * m_InitDir[(UINT)DIR_TYPE::UP];
    GetOwner()->Transform()->SetWorldPos(NewPos);

    // Rotation
    GetOwner()->Transform()->Slerp(m_InitDir[(UINT)DIR_TYPE::FRONT], 1.f);

    // Ready Dir
    m_ReadyDir = m_InitPos - m_InitDir[(UINT)DIR_TYPE::UP] * 300.f;
    m_ReadyDir = (NewPos - m_ReadyDir).Normalize();
}

void CElfilisAirArrow::StartReady()
{
    m_Step = 2;
    m_AccTime = 0.f;
    GetOwner()->GetChildObject()[0]->Transform()->SetLocalScale(Vec3(1.f));
}

void CElfilisAirArrow::StartWait()
{
    m_Step = 3;
    m_AccTime = 0.f;
}

void CElfilisAirArrow::StartAim()
{
    m_Step = 4;
    m_AccTime = 0.f;

    Rigidbody()->SetKinematic(false);
}

void CElfilisAirArrow::StartAttack()
{
    m_Step = 5;
    m_AccTime = 0.f;
    m_bGround = false;

    // Move
    Rigidbody()->SetUseGravity(true);
    Rigidbody()->AddForce(m_ReadyDir * 800.f, ForceMode::Impulse);

    // ==============
    AttackStartPos = Transform()->GetWorldPos();
    TargetPos = m_Target->Transform()->GetWorldPos();
    AttackDir = (TargetPos - AttackStartPos).Normalize();
    AttackFrontDir = (TargetPos - AttackStartPos);
    AttackFrontDir.y = 0.f;
    AttackFrontDir.Normalize();
    DirLock = false;
}

void CElfilisAirArrow::OnCollisionEnter(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() == LAYER_STATIC && m_Step == 5)
    {
        Rigidbody()->SetVelocity(Vec3());
        m_bGround = true;
    }
}

void CElfilisAirArrow::SetInitDir(Vec3 _Front, Vec3 _Up, Vec3 _Right)
{
    m_InitDir[(UINT)DIR_TYPE::FRONT] = _Front;
    m_InitDir[(UINT)DIR_TYPE::UP] = _Up;
    m_InitDir[(UINT)DIR_TYPE::RIGHT] = _Right;
}

void CElfilisAirArrow::SetArrowIdx(int _Idx)
{
    // arrow idx
    m_ArrowIdx = _Idx;

    // wait Time
    switch (m_ArrowIdx)
    {
    case 0:
        m_WaitTime = 2.f;
        break;
    case 1:
        m_WaitTime = 4.f;
        break;
    case 2:
        m_WaitTime = 6.f;
        break;
    case 3:
        m_WaitTime = 7.f;
        break;
    case 4:
        m_WaitTime = 5.f;
        break;
    case 5:
        m_WaitTime = 3.f;
        break;
    case 6:
        m_WaitTime = 1.f;
        break;
    }

    m_WaitTime *= 0.3f;
}

void CElfilisAirArrow::Spawn()
{
    static float SpawnTime = 0.5f;

    // scaling
    if (m_AccTime <= SpawnTime)
    {
        float t = m_AccTime / SpawnTime;
        GetOwner()->GetChildObject()[0]->Transform()->SetLocalScale(Vec3(t));
    }
    else
    {
        GetOwner()->GetChildObject()[0]->Transform()->SetLocalScale(Vec3(1.f));
        m_Step = 0;
    }
}

void CElfilisAirArrow::Ready()
{
    static float ReadyTime = 1.f;

    if (m_AccTime <= ReadyTime)
    {
        static float RotSpeed = 1.f / ReadyTime;

        // rotate (Z & X)
        float t = m_AccTime / ReadyTime;
        float Angle = (-105.f / 7) * (-3 + m_ArrowIdx);
        Vec3 NewRot = Transform()->GetLocalRotation();
        NewRot.z = XMConvertToRadians(Angle) * t;
        NewRot.x = XMConvertToRadians(360.f) * t;
        Transform()->SetLocalRotation(NewRot);

        // pos
        Vec3 NewPos = Transform()->GetLocalPos() + m_ReadyDir * 50.f * DT;
        Transform()->SetLocalPos(NewPos);
    }
    else
    {
        float Angle = (-105.f / 7) * (-3 + m_ArrowIdx);
        Vec3 NewRot = Transform()->GetLocalRotation();
        NewRot.z = XMConvertToRadians(Angle);
        NewRot.x = 0.f;
        Transform()->SetLocalRotation(NewRot);

        StartWait();
    }
}

void CElfilisAirArrow::Wait()
{
    if (m_AccTime > m_WaitTime)
    {
        StartAim();
    }
}

void CElfilisAirArrow::Aim()
{
    static float RotTime = 0.3f;

    if (m_AccTime <= RotTime)
    {
        // rotate
        Vec3 Rotation = GetOwner()->Transform()->GetLocalRotation();
        float t = m_AccTime / RotTime;

        Rotation.y += XMConvertToRadians(360.f) * t;

        GetOwner()->Transform()->SetLocalRotation(Rotation);
    }
    else
    {
        StartAttack();
    }
}

void CElfilisAirArrow::Attack()
{
    if (!m_bGround)
    {
        // Vec3 directionToTarget = (m_Target->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
        // Vec3 currentDirection = Rigidbody()->GetVelocity().Normalize();

        // static float MaxDiff = 25.f;
        // float Diff = (directionToTarget - currentDirection).Length();
        // float t = min(Diff, MaxDiff) / MaxDiff;
        // Vec3 NewDir = directionToTarget * (1.f - t) + currentDirection * t;
        // Rigidbody()->SetVelocity(NewDir.Normalize() * 60.f);

        //// velocity alignment
        // Vec3 Right = m_InitDir[(UINT)DIR_TYPE::RIGHT];
        // Vec3 Down = NewDir;
        // Vec3 Front = Down.Cross(Right);
        // Front.Normalize();

        // Transform()->Slerp(Front, DT);

        // =================
        if (!DirLock)
        {
            Vec3 UpLookAt = (m_Target->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Normalize();
            Vec3 FrontLookAt = m_InitDir[(UINT)DIR_TYPE::RIGHT].Cross(UpLookAt);
            Transform()->Slerp(FrontLookAt, DT);

            Vec3 CurLookAt = Transform()->GetWorldDir(DIR_TYPE::FRONT);
            if (CurLookAt.Dot(FrontLookAt) >= 0.9f)
            {
                DirLock = true;
                Rigidbody()->AddForce(UpLookAt * 60.f, ForceMode::VelocityChange);
            }
        }
        else
        {
            int a = 0;
        }
    }
    else if (m_bGround)
    {
        GamePlayStatic::DetachObject(GetOwner());

        //@Effect : 터지는 이펙트
    }
}

UINT CElfilisAirArrow::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CElfilisAirArrow::LoadFromLevelFile(FILE* _File)
{
    return 0;
}