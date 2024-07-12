#pragma once
#include "CState.h"

class CKirbyStuffedJump : public CState
{
private:
    float   m_JumpAccTime;
    float   m_MinJumpTime;
    float   m_MaxJumpTime;
    bool    m_bVelocityCut;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStuffedJump)
    CKirbyStuffedJump();
    virtual ~CKirbyStuffedJump();
};
