#pragma once
#include "CState.h"

class CKirbySlideAttack : public CState
{
private:
    float m_PlayTime;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbySlideAttack)
    CKirbySlideAttack();
    virtual ~CKirbySlideAttack();
};
