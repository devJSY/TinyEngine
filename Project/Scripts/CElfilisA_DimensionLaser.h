#pragma once
#include "CBossState.h"
class CElfilisDimensionLaser;

class CElfilisA_DimensionLaser : public CBossState
{
private:
    Ptr<CPrefab> m_DimensionStartPref;
    Ptr<CPrefab> m_DimensionPref;
    CGameObject* m_Dimension[5];
    CElfilisDimensionLaser* m_DimensionStart;
    CElfilisDimensionLaser* m_DimensionScript[5];
    int m_ProgressStep;
    float m_AccTime;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Ready();
    void Start();
    void Progress();
    void End();

    void SpawnDimension(int _Idx);

public:
    CLONE(CElfilisA_DimensionLaser)
    CElfilisA_DimensionLaser();
    virtual ~CElfilisA_DimensionLaser();
};
