#pragma once
#include "CState.h"

class CKirbyStuffedDamage : public CState
{
private:
    float m_InitSpeed;
    float m_JumpPower;
    float m_Duration;
    float m_AccTime;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyStuffedDamage)
    CKirbyStuffedDamage();
    virtual ~CKirbyStuffedDamage();
};