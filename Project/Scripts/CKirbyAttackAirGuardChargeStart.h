#pragma once
#include "CState.h"

class CKirbyAttackAirGuardChargeStart : public CState
{
private:
    float m_PrevGravity;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackAirGuardChargeStart)
    CKirbyAttackAirGuardChargeStart();
    virtual ~CKirbyAttackAirGuardChargeStart();
};
