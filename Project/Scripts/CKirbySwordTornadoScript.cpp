#include "pch.h"
#include "CKirbySwordTornadoScript.h"

CKirbySwordTornadoScript::CKirbySwordTornadoScript()
    : CScript(KIRBYSWORDTORNADOSCRIPT)
    , m_OriginScale(Vec3())
    , m_AccTime(0.f)
    , m_PlayTime(3.f)
{
}

CKirbySwordTornadoScript::CKirbySwordTornadoScript(const CKirbySwordTornadoScript& origin)
    : CScript(origin)
    , m_OriginScale(origin.m_OriginScale)
    , m_AccTime(origin.m_AccTime)
    , m_PlayTime(origin.m_PlayTime)
{
}

CKirbySwordTornadoScript::~CKirbySwordTornadoScript()
{
}

void CKirbySwordTornadoScript::begin()
{
    m_OriginPosition = Transform()->GetWorldPos();
    m_OriginFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
    m_OriginScale = Transform()->GetWorldScale();

    // sound
    m_Sound = L"sound\\wav\\HeroSwordMorpho\\Tornado";
    int RandIdx = GetRandomInt(0, 1);
    m_Sound += to_wstring(RandIdx) + L".wav";
    GamePlayStatic::Play2DSound(m_Sound, 1, SOUND_KIRBY * 0.6f);
}

void CKirbySwordTornadoScript::tick()
{
    m_AccTime += DT;

    // Spawn
    float SpawnTime = 0.3f;

    if (m_AccTime <= SpawnTime)
    {
        float t = m_AccTime / SpawnTime;
        Transform()->SetLocalScale(m_OriginScale * t);
    }

    // Rotation
    Quat Quaternion = Quat::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), DT * 20.f);
    Transform()->SetWorldRotation(Transform()->GetWorldQuaternion() * Quaternion);

    // Move
    Vec3 FrontMove = m_OriginFront * m_AccTime * 100.f;
    Vec3 RightMove = m_OriginRight * sinf(m_AccTime * 5.f) * 50.f;

    Transform()->SetWorldPos(m_OriginPosition + FrontMove + RightMove);

    // End
    float EndTime = 0.3f;

    if (m_AccTime > m_PlayTime - EndTime)
    {
        float t = clamp(1.f - (m_AccTime - (m_PlayTime - EndTime)) / EndTime, 0.f, 1.f);
        Transform()->SetLocalScale(m_OriginScale * Vec3(t, 1.f, t));
    }

    // Destroy
    if (m_AccTime > m_PlayTime)
    {
        Transform()->SetLocalScale(m_OriginScale * Vec3(0.f, 1.f, 0.f));
        GamePlayStatic::DestroyGameObject(GetOwner());
        
        // Sound
        CSoundMgr::GetInst()->FadeSound(m_Sound, SOUND_KIRBY * 0.6f, 0.f, 0.5f, false);
    }
}

UINT CKirbySwordTornadoScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CKirbySwordTornadoScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
