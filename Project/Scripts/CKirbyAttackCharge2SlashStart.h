#pragma once
#include "CState.h"

class CKirbyAttackCharge2SlashStart : public CState
{
private:
    float m_PrevSpeed;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackCharge2SlashStart)
    CKirbyAttackCharge2SlashStart();
    virtual ~CKirbyAttackCharge2SlashStart();
};
