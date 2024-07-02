#pragma once
#include "CState.h"

class CKirbyAttackCharge1End : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge1End)
    CKirbyAttackCharge1End();
    virtual ~CKirbyAttackCharge1End();
};
