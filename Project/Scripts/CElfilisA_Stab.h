#pragma once
#include "CBossState.h"

class CElfilisA_Stab : public CBossState
{
private:
    Ptr<CPrefab> m_StabRockPref;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Ready();
    void Start();
    void Progress();
    void Wait();
    void End();

public:
    CLONE(CElfilisA_Stab)
    CElfilisA_Stab();
    virtual ~CElfilisA_Stab();
};