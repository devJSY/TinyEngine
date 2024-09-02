#pragma once
#include "CUnitScript.h"

class CMorphoUnit : public CUnitScript
{
private:
    Ptr<CPrefab> m_LightningEffectPref;

public:
    virtual void tick() override;
    // virtual void AttackReward();

    CGameObject* SpawnAttackButterflyEffect(Vec3 _Pos);
    CGameObject* SpawnLightningEffect(Vec3 _Pos);
    CGameObject* SpawnCircleDustEffect(Vec3 _Pos);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CMorphoUnit)
    CMorphoUnit();
    CMorphoUnit(const CMorphoUnit& _Origin);
    virtual ~CMorphoUnit();
};
