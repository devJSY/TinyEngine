#pragma once
#include "CState.h"

class CKirbyStuffedJumpEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStuffedJumpEnd)
    CKirbyStuffedJumpEnd();
    virtual ~CKirbyStuffedJumpEnd();
};
