#pragma once
#include "CState.h"

class CKirbyAttackCharge3Start : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge3Start)
    CKirbyAttackCharge3Start();
    virtual ~CKirbyAttackCharge3Start();
};
