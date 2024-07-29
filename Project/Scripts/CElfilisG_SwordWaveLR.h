#pragma once
#include "CBossState.h"

class CElfilisG_SwordWaveLR : public CBossState
{
private:
    Ptr<CPrefab> m_SwordSlash;
    bool m_bComboSuccess;
    bool m_bFrmEnter;

public:
    virtual void tick() override;
    virtual void Enter() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Wait();
    void Progress();
    void End();

public:
    CLONE(CElfilisG_SwordWaveLR)
    CElfilisG_SwordWaveLR();
    virtual ~CElfilisG_SwordWaveLR();
};
