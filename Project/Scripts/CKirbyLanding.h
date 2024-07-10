#pragma once
#include "CState.h"

class CKirbyLanding : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLanding)
    CKirbyLanding();
    virtual ~CKirbyLanding();
};
