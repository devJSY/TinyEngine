#include "pch.h"
#include "CElfilisAirArrow.h"
#include "CMomentaryObjScript.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

CElfilisAirArrow::CElfilisAirArrow()
    : CScript(ELFILISAIRARROW)
    , m_Target(nullptr)
    , m_Type(ArrowType::UP)
    , m_ArrowIdx(0)
    , m_Step(0)
    , m_AccTime(0.f)
    , m_AttackSpeed(0.f)
    , m_TargetDist(0.f)
    , m_bGround(false)
{
    m_CollisionEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_ElfilisArrowCol.pref", L"prefab\\Effect_ElfilisArrowCol.pref");
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

    // reset
    GetOwner()->SetActive(true);
    Transform()->SetLocalRotation(Vec3());
    Rigidbody()->SetVelocity(Vec3());
    Rigidbody()->SetKinematic(true);

    // Position
    Vec3 NewPos = m_InitPos;
    int ReflectIdx = abs(m_ArrowIdx - 3);
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
}

void CElfilisAirArrow::StartAttack()
{
    m_Step = 5;
    m_AccTime = 0.f;

    m_bGround = false;
    m_TargetDist = (m_Target->Transform()->GetWorldPos() - Transform()->GetWorldPos()).Length();
}

void CElfilisAirArrow::OnCollisionEnter(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() == LAYER_STATIC && m_Step == 5 && !m_bGround)
    {
        m_bGround = true;
        m_AccTime = 0.f;

        // spawn effect
        if (m_CollisionEffect != nullptr)
        {
            CGameObject* Effect = m_CollisionEffect->Instantiate();
            Vec3 InitPos = Transform()->GetWorldPos();
            InitPos.y -= 40.f;
            Effect->Transform()->SetWorldPos(InitPos);

            CMomentaryObjScript* Script = Effect->GetScript<CMomentaryObjScript>();
            if (Script)
            {
                Script->SetPlayTime(0.5f);
                Script->AddEffect(MomentaryEffectType::AppearScaling);
            }

            GamePlayStatic::SpawnGameObject(Effect, LAYER_EFFECT);
        }
    }
}

void CElfilisAirArrow::OnCollisionStay(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() == LAYER_STATIC && m_Step == 5 && !m_bGround)
    {
        m_bGround = true;
        m_AccTime = 0.f;

        // spawn effect
        if (m_CollisionEffect != nullptr)
        {
            CGameObject* Effect = m_CollisionEffect->Instantiate();
            Vec3 InitPos = Transform()->GetWorldPos();
            InitPos.y -= 40.f;
            Effect->Transform()->SetWorldPos(InitPos);

            CMomentaryObjScript* Script = Effect->GetScript<CMomentaryObjScript>();
            if (Script)
            {
                Script->SetPlayTime(0.5f);
                Script->AddEffect(MomentaryEffectType::AppearScaling);
            }

            GamePlayStatic::SpawnGameObject(Effect, LAYER_EFFECT);
        }
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
    static float ReadyTime = 0.5f;

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

        Rotation.y += XMConvertToRadians(180.f) * t;

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
        static float DetectRange = 200.f;
        Vec3 CurTargetDiff = m_Target->Transform()->GetWorldPos() - Transform()->GetWorldPos();
        float Diff = CurTargetDiff.Length();

        // 추적 : 일정 거리 이상 떨어져있다면 추적
        if (Diff >= DetectRange)
        {
            static float StartTime = 0.5f;
            Vec3 PrevPos = Transform()->GetWorldPos();

            // ~1.f : 현재 UpVector 방향으로 진행
            if (m_AccTime <= StartTime)
            {
                float Ratio = m_AccTime / StartTime;
                m_AttackSpeed = 20.f * sinf(Ratio * XM_PI / 2.f);

                Vec3 MoveDir = m_ReadyDir * m_AttackSpeed;
                Transform()->SetWorldPos(Transform()->GetWorldPos() + MoveDir);
            }

            // 1.f ~ : 타겟방향으로 위치 lerp
            else
            {
                m_AttackSpeed += 300.f * DT;
                float t = m_AttackSpeed / m_TargetDist;

                Vec3 CurPos = Transform()->GetWorldPos();
                Vec3 TargetPos = m_Target->Transform()->GetWorldPos();
                Vec3 NewPos = Vec3::Lerp(CurPos, TargetPos, t);
                Transform()->SetWorldPos(NewPos);

                // 방향회전 : 현재 타겟방향
                Vec3 CurTargetDir = CurTargetDiff.Normalize();
                Quaternion qua = Quaternion::LookRotation(CurTargetDir, Transform()->GetWorldDir(DIR_TYPE::FRONT));
                Quaternion rot = Quaternion::Slerp(Transform()->GetWorldQuaternion(), qua, DT);
                Transform()->SetWorldRotation(rot);
            }

            // 추적 범위 검사 : 일정 거리 이상 가까워졌다면 추적 종료, 현재 방향으로 계속 진행
            Vec3 CurPos = Transform()->GetWorldPos();
            float NewDiff = (m_Target->Transform()->GetWorldPos() - CurPos).Length();

            if (NewDiff < DetectRange)
            {
                Vec3 CurDir = (CurPos - PrevPos).Normalize();
                float Speed = (CurDir).Length() / DT;

                Rigidbody()->SetKinematic(false);
                Rigidbody()->SetVelocity(CurDir * Speed);
            }
        }
    }
    else if (m_bGround)
    {
        Rigidbody()->SetVelocity(Vec3());
        Rigidbody()->SetAngularVelocity(Vec3());

        if (m_AccTime > 1.f)
        {
            GamePlayStatic::DetachObject(GetOwner());
        }
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