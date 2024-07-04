#pragma once
#include "CState.h"

class CKirbyAttackCharge2Start : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge2Start)
    CKirbyAttackCharge2Start();
    virtual ~CKirbyAttackCharge2Start();
};
