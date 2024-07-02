#pragma once
#include "CState.h"

class CKirbyAttackCharge2 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge2)
    CKirbyAttackCharge2();
    virtual ~CKirbyAttackCharge2();
};