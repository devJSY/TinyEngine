#pragma once
#include "CState.h"

class CKirbyLandingEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLandingEnd)
    CKirbyLandingEnd();
    virtual ~CKirbyLandingEnd();
};
