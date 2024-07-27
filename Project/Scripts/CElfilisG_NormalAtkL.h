#pragma once
#include "CState.h"

class CElfilisG_NormalAtkL : public CState
{
private:
    StateStep m_Step;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

private:
    void ChangeStep(StateStep _Step);
    void Enter_Step();
    void Exit_Step();
    
    // tick
    void Start();
    void Progress();
    void End();

public:
    CLONE(CElfilisG_NormalAtkL) CElfilisG_NormalAtkL();
    virtual ~CElfilisG_NormalAtkL();
};