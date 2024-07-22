#pragma once
#include "CState.h"
class CKirbyFinalCutterRise : public CState
{
private:
    float m_SaveGravity;
    float m_SaveJumpPower;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyFinalCutterRise)
    CKirbyFinalCutterRise();
    virtual ~CKirbyFinalCutterRise();

};
