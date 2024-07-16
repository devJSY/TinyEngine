#pragma once
#include "CMonsterUnitScript.h"

class CGordoLongScript : public CMonsterUnitScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CGordoLongScript)
    CGordoLongScript();
    virtual ~CGordoLongScript();
};
