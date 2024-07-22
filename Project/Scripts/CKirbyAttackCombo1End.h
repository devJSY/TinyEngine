#pragma once
#include "CState.h"

class CKirbyAttackCombo1End : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCombo1End)
    CKirbyAttackCombo1End();
    virtual ~CKirbyAttackCombo1End();
};
