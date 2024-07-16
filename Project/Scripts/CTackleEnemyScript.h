#pragma once
#include "CMonsterUnitScript.h"

enum class TACKLEENEMY_STATE
{
    Idle,
    Patrol,
    Find,
    Attack,
    Damage,
    Death,
};

class CTackleEnemyScript : public CMonsterUnitScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    //void ChangeState(NORMALENEMY_STATE _state);
    //void EnterState(NORMALENEMY_STATE _state);
    //void ExitState(NORMALENEMY_STATE _state);

public:
    CLONE(CTackleEnemyScript)
    CTackleEnemyScript();
    virtual ~CTackleEnemyScript();
};
