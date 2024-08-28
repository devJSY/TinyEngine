#include "pch.h"
#include "CElfilisArrowScript.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"

CElfilisArrowScript::CElfilisArrowScript()
    : CScript(ELFILISARROWSCRIPT)
    , m_Step(0)
    , m_AccTime(0.f)
    , m_bSpawnDropStar(false)
{
}

CElfilisArrowScript::~CElfilisArrowScript()
{
}

void CElfilisArrowScript::begin()
{
    GetOwner()->SetActive(false);
    GetOwner()->Transform()->SetLocalScale(Vec3::Zero);
}

void CElfilisArrowScript::tick()
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
        Attack();
        break;
    }
}

void CElfilisArrowScript::Spawn()
{
    // scaling
    static float Speed = 3.f;
    Vec3 Scale = GetOwner()->Transform()->GetLocalScale();
    float NewScale = Scale.x + DT * Speed;

    if (NewScale > 1.f)
    {
        NewScale = 1.f;
        ChangeStep(0);
    }

    GetOwner()->Transform()->SetLocalScale(Vec3(NewScale));
}

void CElfilisArrowScript::Ready()
{
    float RotSpeed = 20.f;

    // rotate & backstep
    if (m_AccTime <= 0.5f)
    {
        Vec3 Position = GetOwner()->Transform()->GetLocalPos();
        Vec3 Rotation = GetOwner()->Transform()->GetLocalRotation();
        Position.z -= DT * 5.f;
        Rotation.z += DT * RotSpeed;

        GetOwner()->Transform()->SetLocalRotation(Rotation);
        GetOwner()->Transform()->SetLocalPos(Position);
    }

    // finish rotation
    else
    {
        Vec3 Rotation = GetOwner()->Transform()->GetLocalRotation();

        if (Rotation.z > 0.1f)
        {
            // Slerp (Z-axis)
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
            ChangeStep(3);
        }
    }
}

void CElfilisArrowScript::Attack()
{
    // move
    static float Speed = 30.f;
    Vec3 Position = GetOwner()->Transform()->GetLocalPos();
    Position.z += DT * Speed;

    GetOwner()->Transform()->SetLocalPos(Position);

    // Spawn DropStar
    if (!m_bSpawnDropStar && m_AccTime >= 0.5f)
    {
        m_bSpawnDropStar = true;
        ELFFSM->SpawnDropStar(GetOwner()->Transform()->GetWorldPos());
    }
}

void CElfilisArrowScript::ChangeStep(UINT _Step)
{
    switch (_Step)
    {
    case 0:
        break;
    case 1: {
        GetOwner()->SetActive(true);
    }
    break;
    case 2:
        break;
    case 3: {
        m_bSpawnDropStar = false;
    }
    break;
    }

    m_AccTime = 0.f;
    m_Step = _Step;
}

UINT CElfilisArrowScript::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CElfilisArrowScript::LoadFromLevelFile(FILE* _File)
{
    return 0;
}