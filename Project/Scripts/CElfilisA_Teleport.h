#pragma once
#include "CBossState.h"

class CElfilisA_Teleport : public CBossState
{
private:
    Ptr<CPrefab> m_Effect;
    CGameObject* m_BeforeObj;
    CGameObject* m_BeforeEffect;
    CGameObject* m_AfterEffect;
    Vec3 m_AfterPos;
    float m_EffectSpeed;

public:
    virtual void tick();

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void End();

public:
    CLONE(CElfilisA_Teleport)
    CElfilisA_Teleport();
    virtual ~CElfilisA_Teleport();
};
