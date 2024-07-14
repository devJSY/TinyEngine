#pragma once
#include "CState.h"
class CBurningEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CBurningEnd)
    CBurningEnd();
    virtual ~CBurningEnd();

};
