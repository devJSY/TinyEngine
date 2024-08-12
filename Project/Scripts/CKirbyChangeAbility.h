#pragma once
#include "CState.h"

class CKirbyChangeAbility : public CState
{
private:
    bool m_bFrameEnter;
    UINT m_SaveSetup;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyChangeAbility)
    CKirbyChangeAbility();
    virtual ~CKirbyChangeAbility();
};
