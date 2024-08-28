#pragma once
#include "CBossState.h"

class CElfilisG_NormalAtkFinishL : public CBossState
{
private:
    bool m_bFrmEnter;

public:
    virtual void tick() override;
    virtual void Exit() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Progress();
    void End();

public:
    CLONE(CElfilisG_NormalAtkFinishL)
    CElfilisG_NormalAtkFinishL();
    virtual ~CElfilisG_NormalAtkFinishL();
};
