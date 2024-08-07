#pragma once
#include "CState.h"

class CElfilisBig_Idle : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CElfilisBig_Idle)
    CElfilisBig_Idle();
    virtual ~CElfilisBig_Idle();
};