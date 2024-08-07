#pragma once
#include "CBossState.h"

class CElfilisG_DimensionSpike : public CBossState
{
private:
    Ptr<CPrefab> m_DimensionSetPref;
    float m_AccTime;
    bool m_bFrmEnter;

public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Ready();
    void ReadyWait();
    void Start();
    void Progress();
    void Wait();
    void End();

public:
    CLONE(CElfilisG_DimensionSpike)
    CElfilisG_DimensionSpike();
    virtual ~CElfilisG_DimensionSpike();
};
