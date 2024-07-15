#pragma once
#include "CState.h"

class CKirbyAttackCharge2SlashEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge2SlashEnd)
    CKirbyAttackCharge2SlashEnd();
    virtual ~CKirbyAttackCharge2SlashEnd();
};
