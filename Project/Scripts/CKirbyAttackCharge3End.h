#pragma once
#include "CState.h"

class CKirbyAttackCharge3End : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge3End)
    CKirbyAttackCharge3End();
    virtual ~CKirbyAttackCharge3End();
};
