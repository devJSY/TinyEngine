#pragma once
#include "CState.h"

class CKirbyStuffedJumpFall : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStuffedJumpFall)
    CKirbyStuffedJumpFall();
    virtual ~CKirbyStuffedJumpFall();
};
