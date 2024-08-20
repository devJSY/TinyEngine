#pragma once
#include "CBossState.h"

class CElfilisD_ResistFail : public CBossState
{
private:
    float m_AccTime;

public:
    virtual void tick() override;

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
