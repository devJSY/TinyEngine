#pragma once
#include "CState.h"

class CKirbyAttackAirGuardCharge : public CState
{
private:
    float   m_PrevGravity;
    float   m_AccTime;
    bool    m_bEnter;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackAirGuardCharge)
    CKirbyAttackAirGuardCharge();
    virtual ~CKirbyAttackAirGuardCharge();
};
