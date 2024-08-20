#pragma once
#include "CState.h"

class CKirbyGuardRun : public CState
{
private:
    Ptr<CPrefab> m_SwordDodgeAttackPref;
    float m_PlayTime;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyGuardRun)
    CKirbyGuardRun();
    virtual ~CKirbyGuardRun();
};