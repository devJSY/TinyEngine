#pragma once
#include "CUnitScript.h"

class CElfilisUnit : public CUnitScript
{
public:
     virtual void tick();

    // virtual void AttackReward();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CElfilisUnit)
    CElfilisUnit();
    CElfilisUnit(const CElfilisUnit& _Origin);
    virtual ~CElfilisUnit();
};
