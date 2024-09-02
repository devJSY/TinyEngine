#pragma once
#include "CUnitScript.h"

class CElfilisUnit : public CUnitScript
{
private:
    Ptr<CPrefab> m_TeleportEffect;

public:
    virtual void tick();

    // virtual void AttackReward();
    void ResistSuccess();

    void PlayTeleportEffect(__inout CGameObject** _BeforeUnit, __inout CGameObject** _BeforeEffect, __inout CGameObject** _AfterEffect, Vec3 _Pos);
    
public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisUnit)
    CElfilisUnit();
    CElfilisUnit(const CElfilisUnit& _Origin);
    virtual ~CElfilisUnit();
};
