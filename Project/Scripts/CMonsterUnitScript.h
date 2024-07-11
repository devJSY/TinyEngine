#pragma once
#include "CUnitScript.h"

class CMonsterUnitScript : public CUnitScript
{
public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    CLONE(CMonsterUnitScript)

    CMonsterUnitScript();
    CMonsterUnitScript(const CMonsterUnitScript& _Origin);
    virtual ~CMonsterUnitScript();
};
