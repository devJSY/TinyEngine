#pragma once
#include <Engine\\CScript.h>

class CMorphoShockWave : public CScript
{
private:
    Ptr<CPrefab> m_WavePref[2];
    float m_AccTime;
    float m_SpawnDuration;
    bool m_bSpawnWave;

public:
    virtual void tick() override;
    void Activate();
    void Deactivate();
    void SetSpawnWave(bool _Spawn) { m_bSpawnWave = _Spawn; }

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;
    void SpawnWave();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CMorphoShockWave);
    CMorphoShockWave();
    virtual ~CMorphoShockWave();
};