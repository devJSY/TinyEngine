#pragma once
#include "CState.h"
class CKirbyLadderExit : public CState
{
public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLadderExit)
    CKirbyLadderExit();
    virtual ~CKirbyLadderExit();
};
