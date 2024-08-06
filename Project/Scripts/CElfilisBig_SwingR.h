#pragma once
#include "CBossState.h"

class CElfilisBig_SwingR : public CBossState
{
private:
    Ptr<CPrefab> m_Rock;
    float m_ProgressChangeDuration;
    bool m_bComboSuccess;
    bool m_bFrmEnter;

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
    CLONE(CElfilisBig_SwingR)
    CElfilisBig_SwingR();
    virtual ~CElfilisBig_SwingR();
};
