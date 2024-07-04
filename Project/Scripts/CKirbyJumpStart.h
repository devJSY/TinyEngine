#pragma once
#include "CState.h"

class CKirbyJumpStart : public CState
{
private:
    bool m_bJumpEnd;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyJumpStart)
    CKirbyJumpStart();
    virtual ~CKirbyJumpStart();
};
