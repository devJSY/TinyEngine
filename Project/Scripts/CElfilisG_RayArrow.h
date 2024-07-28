#pragma once
#include "CBossState.h"

class CElfilisArrowSetScript;

class CElfilisG_RayArrow : public CBossState
{
private:
    Ptr<CPrefab> m_ArrowsPref;
    CGameObject* m_Arrows;
    CElfilisArrowSetScript* m_ArrowSet;

public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Progress();
    void Wait();
    void End();

public:
    CLONE(CElfilisG_RayArrow)
    CElfilisG_RayArrow();
    virtual ~CElfilisG_RayArrow();
};