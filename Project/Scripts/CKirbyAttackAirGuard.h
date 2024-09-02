#pragma once
#include "CState.h"

class CKirbyAttackAirGuard : public CState
{
private:
    Ptr<CPrefab> m_KirbySwordShockWavePref;
    float m_PrevGravity;
    float m_PlayTime;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyAttackAirGuard)
    CKirbyAttackAirGuard();
    virtual ~CKirbyAttackAirGuard();
};
