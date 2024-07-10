#pragma once
#include "CState.h"

class CKirbyStuffedJump : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStuffedJump)
    CKirbyStuffedJump();
    virtual ~CKirbyStuffedJump();
};
