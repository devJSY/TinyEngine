#include "pch.h"
#include "CDestroyParticleScript.h"

CDestroyParticleScript::CDestroyParticleScript()
    : CScript(DESTROYPARTICLESCRIPT)
    , m_SpawnTime(0.3f)
    , m_MaxLifeTime(1.f)
    , m_AccTime(0.f)
    , m_bLive(true)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_SpawnTime, "Spawn Time");
}

CDestroyParticleScript::CDestroyParticleScript(const CDestroyParticleScript& _Origin)
    : CScript(_Origin)
    , m_SpawnTime(_Origin.m_SpawnTime)
    , m_MaxLifeTime(_Origin.m_MaxLifeTime)
    , m_AccTime(0.f)
    , m_bLive(true)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_SpawnTime, "Spawn Time");
}

CDestroyParticleScript::~CDestroyParticleScript()
{
}

void CDestroyParticleScript::begin()
{
    if (!ParticleSystem())
        return;

    ParticleSystem()->EnableModule(PARTICLE_MODULE::SPAWN, true);
    m_MaxLifeTime = ParticleSystem()->GetParticleModule().MaxLife;
}

void CDestroyParticleScript::tick()
{
    if (!ParticleSystem())
        return;

    m_AccTime += DT;

    // Off Spawn
    if (m_bLive && m_AccTime > m_SpawnTime)
    {
        m_bLive = false;
        ParticleSystem()->EnableModule(PARTICLE_MODULE::SPAWN, false);
        m_AccTime = 0.f;
    }

    // Destroy
    if (!m_bLive && m_AccTime > m_MaxLifeTime)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

UINT CDestroyParticleScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_SpawnTime, sizeof(float), 1, _File);
    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CDestroyParticleScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_SpawnTime, sizeof(float), 1, _File);
    MemoryByte += sizeof(float);

    return MemoryByte;
}
