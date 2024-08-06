#pragma once
#include "CState.h"

enum class DOWN_STATE
{
    SLOW,
    FAST,
};

class CKirbyLadderDown : public CState
{
private:
    float m_SaveGravity;
    DOWN_STATE m_DownState;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyLadderDown)
    CKirbyLadderDown();
    virtual ~CKirbyLadderDown();

};
