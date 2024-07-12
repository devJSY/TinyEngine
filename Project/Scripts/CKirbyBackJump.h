#pragma once
#include "CState.h"
class CKirbyBackJump : public CState
{
private:
    float m_InitSpeed;
    float m_JumpPower;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyBackJump)
    CKirbyBackJump();
    virtual ~CKirbyBackJump();
};