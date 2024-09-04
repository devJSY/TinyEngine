#include "pch.h"
#include "CMorphoTrackingSoul.h"
#include "CBossMgr.h"
#include "CMorphoFSM.h"

CMorphoTrackingSoul::CMorphoTrackingSoul()
    : CScript(MORPHOTRACKINGSOUL)
    , m_Target(nullptr)
    , m_Step(StateStep::Start)
    , m_Speed(3.f)
    , m_RotSpeed(0.7f)
    , m_AccTime(0.f)
    , m_bSpawnDropStar(false)
{
}

CMorphoTrackingSoul::~CMorphoTrackingSoul()
{
}

void CMorphoTrackingSoul::begin()
{
    // Sound
    m_Sound = L"sound\\wav\\CharaMorphoknight\\TrackingSoulSound";
    int RandIdx = GetRandomInt(0, 2);
    m_Sound += to_wstring(RandIdx) + L".wav";

    GamePlayStatic::Play2DSound(m_Sound, 0, SOUND_MORPHO * 0.3f);
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
        m_bSpawnDropStar = false;
    }
}

void CMorphoTrackingSoul::Tracking()
{
    Vec3 CurPos = Transform()->GetWorldPos();
    CurPos.y -= 60.f;
    Vec3 Dir = m_Target->Transform()->GetWorldPos() - CurPos;
    Dir.Normalize();
    Vec3 Force = Dir * m_Speed;
    Force = (Rigidbody()->GetVelocity() + Force * m_RotSpeed * DT).Normalize() * m_Speed;

    // chase
    float AppearTime = 2.f;

    if (m_AccTime <= AppearTime)
    {
        float t = m_AccTime / AppearTime;
        float Delta = sinf(XM_PI / 2.f * t);
        Rigidbody()->SetVelocity(Force * Delta);
    }
    else
    {
        Rigidbody()->SetVelocity(Force);
    }

    // rotate
    Dir = Rigidbody()->GetVelocity();
    Dir.y *= 0.3f;
    Dir.Normalize();
    Transform()->Slerp(Dir, m_RotSpeed * DT);

    // spawn dropstar
    if (!m_bSpawnDropStar && m_AccTime > 3.f)
    {
        m_bSpawnDropStar = true;

        Vec3 SpawnPos = Transform()->GetWorldPos();
        SpawnPos.y = 0.f;
        MRPFSM->SpawnDropStar(SpawnPos);
    }

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

        // sound
        GamePlayStatic::StopSound(m_Sound);
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