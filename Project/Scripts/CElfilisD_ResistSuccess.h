#pragma once
#include "CBossState.h"

class CElfilisD_ResistSuccess : public CBossState
{
private:
    bool m_bFrmEnter;

public:
    virtual void tick() override;
    virtual void Exit() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void End();

public:
    CLONE(CElfilisD_ResistSuccess)
    CElfilisD_ResistSuccess();
    virtual ~CElfilisD_ResistSuccess();
};
