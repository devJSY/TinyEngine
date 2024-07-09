#pragma once
#include "CState.h"
class CKirbyDamage : public CState
{
private:
    float m_Acc;
    float m_Duration;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDamage)
    CKirbyDamage();
    virtual ~CKirbyDamage();
};
