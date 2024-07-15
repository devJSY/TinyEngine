#pragma once
#include "CState.h"

class CKirbyAttackCharge3 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge3)
    CKirbyAttackCharge3();
    virtual ~CKirbyAttackCharge3();
};
