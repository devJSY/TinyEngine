#pragma once
#include "CState.h"

class CKirbyAttackCharge2Run : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge2Run)
    CKirbyAttackCharge2Run();
    virtual ~CKirbyAttackCharge2Run();
};
