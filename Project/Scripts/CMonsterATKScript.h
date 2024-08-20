#pragma once
#include <Engine\\CScript.h>

#include "CUnitScript.h"

class CMonsterATKScript : public CScript
{
public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CMonsterATKScript)
public:
    CMonsterATKScript(UINT _Type);
    virtual ~CMonsterATKScript();
};
