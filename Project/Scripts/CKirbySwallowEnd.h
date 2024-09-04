#pragma once
#include "CState.h"
class CKirbySwallowEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbySwallowEnd)
    CKirbySwallowEnd();
    virtual ~CKirbySwallowEnd();

};
