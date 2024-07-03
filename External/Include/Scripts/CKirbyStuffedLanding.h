#pragma once
#include "CState.h"

class CKirbyStuffedLanding : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStuffedLanding)
    CKirbyStuffedLanding();
    virtual ~CKirbyStuffedLanding();
};