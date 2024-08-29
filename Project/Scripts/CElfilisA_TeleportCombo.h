#pragma once
#include "CBossState.h"

class CElfilisA_TeleportCombo : public CBossState
{
private:
    Ptr<CPrefab> m_Effect;
    CGameObject* m_BeforeObj;
    CGameObject* m_BeforeEffect;
    CGameObject* m_AfterEffect;
    Vec3 m_AfterPos;
    float m_EffectSpeed;
    int m_ComboLevel;

public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void End();

public:
    CLONE(CElfilisA_TeleportCombo)
    CElfilisA_TeleportCombo();
    virtual ~CElfilisA_TeleportCombo();
};