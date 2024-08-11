#pragma once
#include "CBossState.h"

class CMorphoAtkA_DoubleSwordAtkL : public CBossState
{
private:
    Ptr<CPrefab> m_LightningEffectPref;
    CGameObject* m_LightningEffect;
    bool m_bFrmEnter;

public:
    virtual void tick() override;

private:
    void Enter_Step();
    void Exit_Step();

    // tick
    void Start();
    void Progress();
    void End();

public:
    CLONE(CMorphoAtkA_DoubleSwordAtkL)
    CMorphoAtkA_DoubleSwordAtkL();
    virtual ~CMorphoAtkA_DoubleSwordAtkL();
};