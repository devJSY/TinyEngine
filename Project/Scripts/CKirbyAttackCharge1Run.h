#pragma once
#include "CState.h"

class CKirbyAttackCharge1Run : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge1Run)
    CKirbyAttackCharge1Run();
    virtual ~CKirbyAttackCharge1Run();
};
