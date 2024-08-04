#pragma once
#include "CBossState.h"

class CElfilisG_ToAirTeleport : public CBossState
{
private:
    Ptr<CPrefab> m_Effect;
    CGameObject* m_BeforeObj;
    CGameObject* m_BeforeEffect;
    CGameObject* m_AfterEffect;
    Vec3 m_AfterPos;
    float m_EffectSpeed;

public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void End();

public:
    CLONE(CElfilisG_ToAirTeleport)
    CElfilisG_ToAirTeleport();
    virtual ~CElfilisG_ToAirTeleport();
};