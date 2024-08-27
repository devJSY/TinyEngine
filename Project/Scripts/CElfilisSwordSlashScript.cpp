#include "pch.h"
#include "CElfilisSwordSlashScript.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"

CElfilisSwordSlashScript::CElfilisSwordSlashScript()
    : CScript(ELFILISSWORDSLASHSCRIPT)
    , m_AccTime(0.f)
    , m_LifeTime(3.f)
    , m_bSpawnDropStar(false)
{
}

CElfilisSwordSlashScript::~CElfilisSwordSlashScript()
{
}

void CElfilisSwordSlashScript::begin()
{
    Transform()->Slerp(m_InitDir, 1.f);

    if (Rigidbody())
    {
        Rigidbody()->AddForce(m_InitDir * 30.f, ForceMode::VelocityChange);
    }
}

void CElfilisSwordSlashScript::tick()
{
    static float ScalingTime = 0.3f;
    m_AccTime += DT;

    // Appear : Scaling Up
    if (m_AccTime < ScalingTime)
    {
        float t = m_AccTime / ScalingTime;
        GetOwner()->GetChildObject()[0]->Transform()->SetLocalScale(Vec3(t));
    }
    
    // End : Spawn DropStar
    if (!m_bSpawnDropStar && m_AccTime > 1.f)
    {
        m_bSpawnDropStar = true;
        Vec3 SpawnPos = GetOwner()->Transform()->GetWorldPos();

        for (int i = 0; i < 3; ++i)
        {
            Vec3 SpaceSpawnPos = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
            SpaceSpawnPos = SpawnPos + SpaceSpawnPos * (-200.f + (200.f * i));
            ELFFSM->SpawnDropStar(SpaceSpawnPos);
        }
    }

    // End : Destroy
    else if (m_AccTime > m_LifeTime)
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

UINT CElfilisSwordSlashScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CElfilisSwordSlashScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}