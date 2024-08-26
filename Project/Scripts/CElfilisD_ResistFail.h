#pragma once
#include "CBossState.h"

class CElfilisD_ResistFail : public CBossState
{
private:
    Ptr<CPrefab> m_StageClearPref;
    float m_AccTime;
    bool m_bFrmEnter;

public:
    virtual void tick() override;
    virtual void Exit() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void Progress();

public:
    CLONE(CElfilisD_ResistFail)
    CElfilisD_ResistFail();
    virtual ~CElfilisD_ResistFail();
};
