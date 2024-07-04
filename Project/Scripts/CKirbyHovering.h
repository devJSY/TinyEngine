#pragma once
#include "CState.h"

class CKirbyHovering : public CState
{
private:
    bool m_bFrmEnter;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyHovering)
    CKirbyHovering();
    virtual ~CKirbyHovering();
};