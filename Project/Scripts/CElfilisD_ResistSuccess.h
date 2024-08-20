#pragma once
#include "CBossState.h"

class CElfilisD_ResistSuccess : public CBossState
{
public:
    virtual void tick() override;

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
