#pragma once
#include "CState.h"

class CKirbyAttackAirGuardEnd : public CState
{
private:
    float m_PrevGravity;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackAirGuardEnd)
    CKirbyAttackAirGuardEnd();
    virtual ~CKirbyAttackAirGuardEnd();
};
