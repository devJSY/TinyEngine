#include "pch.h"
#include "CProjectileScript.h"

CProjectileScript::CProjectileScript()
    : CScript(PROJECTILESCRIPT)
    , m_InitVelocity(Vec3())
{
}

CProjectileScript::~CProjectileScript()
{
}

void CProjectileScript::begin()
{
    if (Rigidbody())
        Rigidbody()->AddForce(m_InitVelocity, ForceMode::Impulse);
}

void CProjectileScript::tick()
{
}

void CProjectileScript::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_InitVelocity, sizeof(Vec3), 1, _File);
}

void CProjectileScript::LoadFromLevelFile(FILE* _File)
{
    fread(&m_InitVelocity, sizeof(Vec3), 1, _File);
}