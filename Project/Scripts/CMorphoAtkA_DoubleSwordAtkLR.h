#pragma once
#include "CBossState.h"

class CMorphoAtkA_DoubleSwordAtkLR : public CBossState
{
    Ptr<CPrefab> m_LightningEffectPref;
    Ptr<CPrefab> m_FireSwipePref;
    CGameObject* m_LightningEffect[2];
    CGameObject* m_FireSwipe[2];
    Vec3 m_SwipeOriginScale;
    Vec3 m_SwipeTargetDir;
    float m_AccTime;
    bool m_bFrmEnter;
    bool m_bOverlapBurning;

public:
    virtual void tick() override;
    virtual void Exit() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void Progress();
    void End();
    void EndWait();
    void EndEnd();

    void SpawnCircleDust(Vec3 _Pos);
    void SpawnButterfly(Vec3 _Pos);

public:
    CLONE(CMorphoAtkA_DoubleSwordAtkLR)
    CMorphoAtkA_DoubleSwordAtkLR();
    virtual ~CMorphoAtkA_DoubleSwordAtkLR();
};
