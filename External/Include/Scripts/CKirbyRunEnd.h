#pragma once
#include "CState.h"

class CKirbyRunEnd : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyRunEnd)
    CKirbyRunEnd();
    virtual ~CKirbyRunEnd();
};