#pragma once
#include "CState.h"

class CKirbyRunStart : public CState
{
private:
    bool m_LastSmokeIsRight;


public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

private:
    void SpawnSmoke();


public:
    CLONE(CKirbyRunStart)
    CKirbyRunStart();
    virtual ~CKirbyRunStart();
};
