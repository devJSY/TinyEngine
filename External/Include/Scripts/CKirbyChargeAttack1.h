#pragma once
#include "CState.h"

class CKirbyChargeAttack1 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyChargeAttack1)
    CKirbyChargeAttack1();
    virtual ~CKirbyChargeAttack1();
};
