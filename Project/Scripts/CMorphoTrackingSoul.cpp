#include "pch.h"
#include "CMorphoTrackingSoul.h"

CMorphoTrackingSoul::CMorphoTrackingSoul()
    : CScript(MORPHOTRACKINGSOUL)
    , m_Target(nullptr)
    , m_Step(StateStep::Start)
    , m_Speed(200.f)
    , m_RotSpeed(3.f)
    , m_AccTime(0.f)
{
}

CMorphoTrackingSoul::~CMorphoTrackingSoul()
{
}

void CMorphoTrackingSoul::begin()
{
}

void CMorphoTrackingSoul::tick()
{
    if (!m_Target)
        return;

    m_AccTime += DT;

    switch (m_Step)
    {
    case StateStep::Start: {
        Appear();
    }
    break;
    case StateStep::Progress: {
        Tracking();
    }
    break;
    case StateStep::End: {
        Disappear();
    }
    break;
    }
}

void CMorphoTrackingSoul::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner() != m_Target)
        return;

    m_AccTime = 0.f;
    m_Step = StateStep::End;
}

void CMorphoTrackingSoul::Appear()
{
    float AppearTime = 0.5f;

    // scaling
    float t = clamp(m_AccTime / AppearTime, 0.f, 1.f);
    GetOwner()->GetChildObject()[0]->Transform()->SetLocalScale(Vec3(t));

    // change state
    if (m_AccTime > AppearTime)
    {
        m_Step = StateStep::Progress;
        m_AccTime = 0.f;
    }
}

void CMorphoTrackingSoul::Tracking()
{
    Vec3 CurPos = Transform()->GetWorldPos();
    CurPos.y -= 60.f;
    Vec3 Dir = m_Target->Transform()->GetWorldPos() - CurPos;
    Dir.Normalize();

    // chase
    Vec3 Veloc = Rigidbody()->GetVelocity();
    Vec3 Force = Dir * m_Speed;

    if (Veloc.Length() <= m_Speed)
    {
        Rigidbody()->AddForce(Force, ForceMode::Force);
    }
    else
    {
        Force = (Veloc + Force * DT).Normalize() * m_Speed;
        Rigidbody()->SetVelocity(Force);
    }

    // rotate
    Dir = Rigidbody()->GetVelocity();
    Dir.y *= 0.3f;
    Dir.Normalize();
    Transform()->Slerp(Dir, m_RotSpeed * DT);

    // change state
    float LifeTime = 14.f;
    if (m_AccTime > LifeTime)
    {
        m_Step = StateStep::End;
        m_AccTime = 0.f;
    }
}

void CMorphoTrackingSoul::Disappear()
{
    float DisappearTime = 1.f;

    // move front
    Vec3 Veloc = Rigidbody()->GetVelocity();
    Vec3 Force = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    Force.y = 0.f;
    Force *= m_Speed;

    if (Veloc.Length() <= m_Speed)
    {
        Rigidbody()->AddForce(Force, ForceMode::Force);
    }
    else
    {
        Rigidbody()->SetVelocity(Force);
    }

    // scaling
    float t = 1.f - clamp(m_AccTime / DisappearTime, 0.f, 1.f);
    GetOwner()->GetChildObject()[0]->Transform()->SetLocalScale(Vec3(t));

    // change state
    if (m_AccTime > DisappearTime)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

UINT CMorphoTrackingSoul::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CMorphoTrackingSoul::LoadFromLevelFile(FILE* _File)
{
    return 0;
}