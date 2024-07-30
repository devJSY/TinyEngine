#pragma once
#include "CState.h"

class CKirbyAttack : public CState
{
private:
    float m_AccTime;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttack)
    CKirbyAttack();
    virtual ~CKirbyAttack();
};
