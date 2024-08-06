#pragma once
#include "CBossState.h"

class CElfilisG_SwordWaveFinishRL : public CBossState
{
private:
    Ptr<CPrefab> m_SwordSlash;
    bool m_bFrmEnter;

public:
    virtual void tick() override;
    virtual void Enter() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Wait();
    void Progress();

public:
    CLONE(CElfilisG_SwordWaveFinishRL)
    CElfilisG_SwordWaveFinishRL();
    virtual ~CElfilisG_SwordWaveFinishRL();
};
