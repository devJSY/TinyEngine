#pragma once
#include "CState.h"

class CKirbyAttackCombo2 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCombo2)
    CKirbyAttackCombo2();
    virtual ~CKirbyAttackCombo2();
};