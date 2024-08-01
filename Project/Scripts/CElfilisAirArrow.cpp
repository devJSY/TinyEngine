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
    m_OriginScale = Transform()->GetLocalScale();

    GetOwner()->SetActive(false);
    GetOwner()->Transform()->SetLocalScale(Vec3());

    // Position
    Vec3 NewPos = m_InitPos;
    NewPos.x += 60.f * (-3 + m_ArrowIdx);
    NewPos.y += 45.f * (-3 + m_ArrowIdx);
    GetOwner()->Transform()->SetWorldPos(NewPos);

    // Rotation
    Vec3 NewRot = Vec3(XMConvertToRadians(90.f), 0.f, 0.f);
    GetOwner()->Transform()->SetLocalRotation(NewRot);
    GetOwner()->Transform()->Slerp(m_InitFront, 1.f);
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
}

void CElfilisAirArrow::StartReady()
{
    m_Step = 2;
    m_AccTime = 0.f;

    m_ReadyRot = Transform()->GetLocalRotation();
    m_ReadyRot.z += XMConvertToRadians((105.f / 7) * (-3 + m_ArrowIdx));
}

void CElfilisAirArrow::StartAim()
{
    m_Step = 3;
}

void CElfilisAirArrow::Spawn()
{
    static float SpawnTime = 0.5f;
    m_AccTime += DT;

    // scaling
    if (m_AccTime <= SpawnTime)
    {
        float t = m_AccTime / SpawnTime;
        GetOwner()->Transform()->SetLocalScale(m_OriginScale * t);
    }
}

void CElfilisAirArrow::Ready()
{
    // NewRot = m_StormWhite->Transform()->GetLocalRotation();
    // NewRot.y -= XMConvertToRadians(360.f * 8.f * DT);
    // m_StormWhite->Transform()->SetLocalRotation(NewRot);

    static float ReadyTime = 1.f;
    m_AccTime += DT;

    if (m_AccTime <= ReadyTime)
    {
        // rotate Z
        //float t = m_AccTime / ReadyTime;
        float Target = XMConvertToRadians((105.f / 7) * (-3 + m_ArrowIdx));
        Target /= ReadyTime;

        Vec3 NewRot = Transform()->GetLocalRotation();
        NewRot.z += Target * DT;

        // rotate X
        

        // ======================================================
        Vec3 Position = GetOwner()->Transform()->GetLocalPos();
        Vec3 Rotation = GetOwner()->Transform()->GetLocalRotation();
        Position.z -= DT;
        Rotation.z += DT * RotSpeed;

        GetOwner()->Transform()->SetLocalRotation(Rotation);
        GetOwner()->Transform()->SetLocalPos(Position);
    }
    else
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