#pragma once
#include <Engine\CScript.h>

enum class SpawnTriggerState
{
    Active,
    Spawn,
    DeActive,
    End,
};

class CSpawnTriggerScript : public CScript
{
public:
    CGameObject* m_pSpawnMonster[4];
    int m_iSpawnMonster[4];
    SpawnTriggerState m_eState;

public:
    virtual void begin() override;

private:
    void ChangeState(SpawnTriggerState _state);
    void EnterState();
    void ExitState();

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider) override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CSpawnTriggerScript)

public:
    CSpawnTriggerScript();
    CSpawnTriggerScript(const CSpawnTriggerScript& Origin);
    virtual ~CSpawnTriggerScript();
};
