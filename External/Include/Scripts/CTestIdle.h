#pragma once
#include "CState.h"
class CTestIdle :
    public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CTestIdle)
    CTestIdle();
    virtual ~CTestIdle();
};

