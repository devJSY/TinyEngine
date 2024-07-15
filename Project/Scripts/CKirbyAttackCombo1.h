#pragma once
#include "CState.h"

class CKirbyAttackCombo1 : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCombo1)
    CKirbyAttackCombo1();
    virtual ~CKirbyAttackCombo1();
};