#pragma once
#include "CState.h"

class CKirbyAttackCharge2Slash : public CState
{
private:
    float m_Speed;
    float m_PrevSpeed;
    float m_PlayTime;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge2Slash)
    CKirbyAttackCharge2Slash();
    virtual ~CKirbyAttackCharge2Slash();
};