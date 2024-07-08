#pragma once
#include "CState.h"

class CKirbyDodge1 : public CState
{
private:
    float m_InitSpeed;
    float m_JumpPower;
    float m_DodgeSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDodge1)
    CKirbyDodge1();
    virtual ~CKirbyDodge1();
};
