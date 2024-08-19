#pragma once
#include "CState.h"
class CKirbyDodge2 : public CState
{
private:
    float m_InitSpeed;
    float m_JumpPower;
    float m_DodgeSpeed;
    float m_SaveGravity;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodge2)
    CKirbyDodge2();
    virtual ~CKirbyDodge2();
};
