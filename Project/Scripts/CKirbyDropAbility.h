#pragma once
#include "CState.h"

class CKirbyDropAbility : public CState
{
private:
    Ptr<CPrefab>    m_Bubble;
    bool            m_FrmEnter;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyDropAbility)
    CKirbyDropAbility();
    virtual ~CKirbyDropAbility();
};
