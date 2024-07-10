#pragma once
#include "CState.h"

class CKirbyAttackCharge1Start : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge1Start)
    CKirbyAttackCharge1Start();
    virtual ~CKirbyAttackCharge1Start();
};
