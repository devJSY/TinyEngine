#pragma once
#include "CState.h"
class CKirbyFinalCutterEndAfter : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyFinalCutterEndAfter)
    CKirbyFinalCutterEndAfter();
    virtual ~CKirbyFinalCutterEndAfter();
};
