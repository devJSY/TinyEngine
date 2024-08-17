#pragma once

#include <Engine/CScript.h>

enum class SpawnType
{
    Periodic, 
    Random, 
};

enum class SpawnMesh
{
    Sphere,
    Box,
    END,
};

class CSmokeSpawner : public CScript
{
private:
    Ptr<CPrefab> m_SmokePrefab;
    string      m_SmokePrefabName;  // Spawn«“ Smoke¿« Prefab

    bool        m_bIsOneTime;        
    bool        m_bRandom;         


    // Periodic
    float       m_SpawnCoolTime;
    float       m_Acc;

    // Random
    UINT        m_SpawnMesh;
    int         m_MinSpawnCount;
    int         m_MaxSpawnCount;

private:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Spawn();
    float SimpleNoise(float min, float max);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CSmokeSpawner);
    CSmokeSpawner();
    CSmokeSpawner(const CSmokeSpawner& _Origin);
    virtual ~CSmokeSpawner();

};
