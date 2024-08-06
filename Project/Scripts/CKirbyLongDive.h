#pragma once
#include "CState.h"
class CKirbyLongDive : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLongDive)
    CKirbyLongDive();
    virtual ~CKirbyLongDive();
};
