#pragma once
#include "CState.h"

class CKirbyJumpStart : public CState
{
private:
    float m_JumpAccTime;
    float m_MinJumpTime;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyJumpStart)
    CKirbyJumpStart();
    virtual ~CKirbyJumpStart();
};
