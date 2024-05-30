#pragma once
#include "CState.h"
class CTestLeft :
    public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CTestLeft)
    CTestLeft();
    virtual ~CTestLeft();
};

