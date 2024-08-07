#pragma once
#include "CBossState.h"
class CElfilisLaser;

class CElfilisA_DrawLaser : public CBossState
{
private:
    Ptr<CPrefab> m_LaserPref;
    CElfilisLaser* m_LaserScript;
    float m_AccTime;

public:
    virtual void tick() override;
    virtual void Enter() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Ready();
    void Charge();
    void Start();
    void Progress();
    void End();

public:
    CLONE(CElfilisA_DrawLaser)
    CElfilisA_DrawLaser();
    virtual ~CElfilisA_DrawLaser();
};