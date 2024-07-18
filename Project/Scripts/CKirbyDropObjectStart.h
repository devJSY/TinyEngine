#pragma once
#include "CState.h"

class CKirbyDropObjectStart : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDropObjectStart)
    CKirbyDropObjectStart();
    virtual ~CKirbyDropObjectStart();
};