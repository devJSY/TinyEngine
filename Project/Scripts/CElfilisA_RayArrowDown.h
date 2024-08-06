#pragma once
#include "CBossState.h"
class CElfilisArrowSetScript;

class CElfilisA_RayArrowDown : public CBossState
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
    CLONE(CElfilisA_RayArrowDown)
    CElfilisA_RayArrowDown();
    virtual ~CElfilisA_RayArrowDown();
};
