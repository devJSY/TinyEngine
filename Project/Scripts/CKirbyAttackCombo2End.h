#pragma once
#include "CState.h"

class CKirbyAttackCombo2End : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCombo2End)
    CKirbyAttackCombo2End();
    virtual ~CKirbyAttackCombo2End();
};
