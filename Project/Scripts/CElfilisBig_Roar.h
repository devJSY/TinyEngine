#pragma once
#include "CState.h"

class CElfilisBig_Roar : public CState
{
private:
    float m_AccTime;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CElfilisBig_Roar)
    CElfilisBig_Roar();
    virtual ~CElfilisBig_Roar();
};
