#pragma once
#include "CUnitScript.h"

class CMorphoUnit : public CUnitScript
{
public:
    // virtual void tick();

    // virtual void AttackReward();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CMorphoUnit)
    CMorphoUnit();
    virtual ~CMorphoUnit();
};
