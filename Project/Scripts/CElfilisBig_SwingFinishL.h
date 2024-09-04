#pragma once
#include "CBossState.h"

class CElfilisBig_SwingFinishL : public CBossState
{
private:
    Ptr<CPrefab> m_Rock;
    float m_AccTime;
    float m_PrevAlpha;
    bool m_bFrmEnter;

    // sound
    UINT m_SoundIdx;

public:
    virtual void tick() override;
    virtual void Enter() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Wait();
    void Progress();
    void End();
    void EndWait();

public:
    CLONE(CElfilisBig_SwingFinishL)
    CElfilisBig_SwingFinishL();
    virtual ~CElfilisBig_SwingFinishL();
};
