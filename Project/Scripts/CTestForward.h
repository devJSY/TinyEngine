#pragma once
#include "CState.h"
class CTestForward : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CTestForward)
    CTestForward();
    virtual ~CTestForward();
};
