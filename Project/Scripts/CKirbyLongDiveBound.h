#pragma once
#include "CState.h"
class CKirbyLongDiveBound : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLongDiveBound)
    CKirbyLongDiveBound();
    virtual ~CKirbyLongDiveBound();
};
