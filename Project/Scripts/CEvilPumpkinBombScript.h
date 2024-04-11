#pragma once
#include "CEnemyScript.h"

enum class EVILPUMPKINBOMB_STATE
{
    Appear,
    Explode,
};

class CEvilPumpkinBombScript : public CEnemyScript
{
private:
    EVILPUMPKINBOMB_STATE m_State;
    bool m_bInitForce;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual bool TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3()) override { return false; };

public:
    void ChangeState(EVILPUMPKINBOMB_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Appear();
    void Explode();

public:
    CLONE(CEvilPumpkinBombScript);

public:
    CEvilPumpkinBombScript();
    virtual ~CEvilPumpkinBombScript();

    friend class CEvilPumpkinScript;
};
