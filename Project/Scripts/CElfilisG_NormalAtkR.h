#pragma once
#include "CBossState.h"

class CElfilisG_NormalAtkR : public CBossState
{
private:
    bool m_bComboSuccess;

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
    CLONE(CElfilisG_NormalAtkR)
    CElfilisG_NormalAtkR();
    virtual ~CElfilisG_NormalAtkR();
};