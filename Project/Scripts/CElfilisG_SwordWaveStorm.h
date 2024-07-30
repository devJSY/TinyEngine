#pragma once
#include "CBossState.h"

class CElfilisStormScript;

class CElfilisG_SwordWaveStorm : public CBossState
{
private:
    Ptr<CPrefab> m_StormPref;
    CElfilisStormScript* m_Storm;
    float m_AccTime;
    float m_PlayTime;
    bool m_bFrmEnter;

public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Progress();
    void Wait();
    void End();

public:
    CLONE(CElfilisG_SwordWaveStorm)
    CElfilisG_SwordWaveStorm();
    virtual ~CElfilisG_SwordWaveStorm();
};
