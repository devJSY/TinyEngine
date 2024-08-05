#pragma once
#include "CBossState.h"

class CElfilisG_NormalAtkTeleportFinishL : public CBossState
{
private:
    Ptr<CPrefab> m_Effect;
    CGameObject* m_BeforeObj;
    CGameObject* m_BeforeEffect;
    CGameObject* m_AfterEffect;
    Vec3 m_AfterPos;
    float m_EffectSpeed;
    bool m_bFrmEnter;

public:
    virtual void tick() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void StartEnd();
    void Progress();
    void End();

    void SpawnTeleport();

public:
    CLONE(CElfilisG_NormalAtkTeleportFinishL)
    CElfilisG_NormalAtkTeleportFinishL();
    virtual ~CElfilisG_NormalAtkTeleportFinishL();
};
