#pragma once
#include "CBossState.h"

class CElfilisG_NormalAtkL : public CBossState
{
public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Progress();
    void End();

public:
    CLONE(CElfilisG_NormalAtkL)
    CElfilisG_NormalAtkL();
    virtual ~CElfilisG_NormalAtkL();
};