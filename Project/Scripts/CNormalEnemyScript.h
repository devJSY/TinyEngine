#pragma once
#include <Engine/CScript.h>

#include "CMonsterUnitScript.h"

enum class NORMALENEMY_STATE
{
    Idle,
    Sleep,
    Patrol,
    Find,
    Attack,
    AttackSuccessed,
    AttackFailed,
    End,
};

class CNormalEnemyScript : public CMonsterUnitScript
{
private:
    NORMALENEMY_STATE m_eState;
    

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    void ChangeState(NORMALENEMY_STATE _state);
    void EnterState(NORMALENEMY_STATE _state);
    void ExitState(NORMALENEMY_STATE _state);

private:
    void Idle();

public:
    CLONE(CNormalEnemyScript)
    CNormalEnemyScript();
    virtual ~CNormalEnemyScript();
};
