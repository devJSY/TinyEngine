#pragma once
#include "CState.h"
class CKirbySwallowStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbySwallowStart)
    CKirbySwallowStart();
    virtual ~CKirbySwallowStart();

};
