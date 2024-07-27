#pragma once
#include "CBossState.h"

class CElfilisG_Teleport : public CBossState
{
public:
    virtual void tick();

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void Progress();
    void End();

public:
    CLONE(CElfilisG_Teleport)
    CElfilisG_Teleport();
    virtual ~CElfilisG_Teleport();
};
