#include "pch.h"
#include "CElfilisAirArrow.h"

CElfilisAirArrow::CElfilisAirArrow()
    : CScript(ELFILISAIRARROW)
    , m_Type(ArrowType::UP)
    , m_ArrowIdx(0)
    , m_Step(0)
    , m_AccTime(0.f)
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
        Aim();
        break;
    case 4:
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

void CElfilisAirArrow::StartAim()
{
    m_Step = 3;

    Rigidbody()->SetKinematic(false);
}

void CElfilisAirArrow::SetInitDir(Vec3 _Front, Vec3 _Up, Vec3 _Right)
{
    m_InitDir[(UINT)DIR_TYPE::FRONT] = _Front;
    m_InitDir[(UINT)DIR_TYPE::UP] = _Up;
    m_InitDir[(UINT)DIR_TYPE::RIGHT] = _Right;
}

void CElfilisAirArrow::Spawn()
{
    static float SpawnTime = 0.5f;
    m_AccTime += DT;

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
    m_AccTime += DT;

    if (m_AccTime <= ReadyTime)
    {
        static float RotSpeed = 1.f / ReadyTime;

        // roatate (Upvector & X) 
        float t = m_AccTime / ReadyTime;
        float Angle = (105.f / 7) * (-3 + m_ArrowIdx) * RotSpeed * DT;
        Vec3 NewRot = Transform()->GetLocalRotation();
        NewRot += m_InitDir[(UINT)DIR_TYPE::FRONT] * XMConvertToRadians(Angle);
        NewRot.x = XMConvertToRadians(360.f) * t;
        Transform()->SetLocalRotation(NewRot);

        // pos
        Vec3 NewPos = Transform()->GetLocalPos() + m_ReadyDir * 50.f * DT;
        Transform()->SetLocalPos(NewPos);
    }
    else
    {
    }
}

void CElfilisAirArrow::Aim()
{
    static float RotSpeed = 20.f;
    m_AccTime += DT;

    if (m_AccTime >= 0.5f)
    {
        // finish rotation
        Vec3 Rotation = GetOwner()->Transform()->GetLocalRotation();

        if (Rotation.z > 0.1f)
        {
            // Slerp
            float _t = DT * RotSpeed;
            Vec3 _TowardDir = Vec3();

            Vec3 Up = Vec3(0.f, 0.f, 1.f);
            Vec3 Right = Up.Cross(_TowardDir);
            Right.Normalize();
            Up = _TowardDir.Cross(Right);
            Up.Normalize();

            Matrix RotationMatrix = Matrix();
            RotationMatrix.Forward(-_TowardDir);
            RotationMatrix.Up(Up);
            RotationMatrix.Right(Right);

            Quat SlerpQuat = Quat::Slerp(GetOwner()->Transform()->GetWorldQuaternion(), Quat::CreateFromRotationMatrix(RotationMatrix), _t);
            RotationMatrix = Matrix::CreateFromQuaternion(SlerpQuat);
            Vec3 LookDir = -RotationMatrix.Forward();
            GetOwner()->Transform()->SetDirection(LookDir);
        }
        else
        {
            GetOwner()->Transform()->SetLocalRotation(Vec3());
            m_Step = 3;
        }
    }
    else
    {
        // rotate & backstep
        Vec3 Position = GetOwner()->Transform()->GetLocalPos();
        Vec3 Rotation = GetOwner()->Transform()->GetLocalRotation();
        Position.z -= DT;
        Rotation.z += DT * RotSpeed;

        GetOwner()->Transform()->SetLocalRotation(Rotation);
        GetOwner()->Transform()->SetLocalPos(Position);
    }
}

void CElfilisAirArrow::Attack()
{
    // move
    static float Speed = 70.f;
    Vec3 Position = GetOwner()->Transform()->GetLocalPos();
    Position.z += DT * Speed;

    GetOwner()->Transform()->SetLocalPos(Position);
}

UINT CElfilisAirArrow::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CElfilisAirArrow::LoadFromLevelFile(FILE* _File)
{
    return 0;
}