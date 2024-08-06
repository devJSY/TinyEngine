#pragma once
#include "CState.h"

class CKirbyDeath : public CState
{
private:
    float m_Acc;
    float m_Duration;
    float m_FaceDuraion;
    float m_DeathDuraion;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDeath)
    CKirbyDeath();
    virtual ~CKirbyDeath();
};