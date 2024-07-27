#pragma once
#include "CState.h"

class CElfilisG_Idle : public CState
{
private:
    float m_AccTime;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CElfilisG_Idle)
    CElfilisG_Idle();
    virtual ~CElfilisG_Idle();
};