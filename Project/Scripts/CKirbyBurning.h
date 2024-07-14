#pragma once
#include "CState.h"
class CKirbyBurning : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyBurning)
    CKirbyBurning();
    virtual ~CKirbyBurning();
};
