#pragma once
#include "CState.h"

class CElfilisBig_Appear : public CState
{
private:
    float m_AccTime;
    float m_PrevAlpha;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CElfilisBig_Appear)
    CElfilisBig_Appear();
    virtual ~CElfilisBig_Appear();
};