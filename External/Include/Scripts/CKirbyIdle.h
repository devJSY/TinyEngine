#pragma once
#include "CState.h"

class CKirbyIdle : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyIdle)
    CKirbyIdle();
    virtual ~CKirbyIdle();
};