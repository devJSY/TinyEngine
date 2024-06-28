#pragma once
#include "CState.h"

class CKirbyAttackCharge1 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge1)
    CKirbyAttackCharge1();
    virtual ~CKirbyAttackCharge1();
};
