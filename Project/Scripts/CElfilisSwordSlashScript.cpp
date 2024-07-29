#include "pch.h"
#include "CElfilisSwordSlashScript.h"

CElfilisSwordSlashScript::CElfilisSwordSlashScript()
    : CScript(ELFILISSWORDSLASHSCRIPT)
    , m_AccTime(0.f)
    , m_LifeTime(3.f)
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
        Rigidbody()->AddForce(m_InitDir * 800.f, ForceMode::Impulse);
    }
}

void CElfilisSwordSlashScript::tick()
{
    static float ScalingTime = 0.3f;
    m_AccTime += DT;

    if (m_AccTime < ScalingTime)
    {
        float t = m_AccTime / ScalingTime;
        GetOwner()->GetChildObject()[0]->Transform()->SetLocalScale(Vec3(t));
    }

    if (m_AccTime > m_LifeTime)
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