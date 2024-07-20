#pragma once
#include <Engine\\CScript.h>

#include "CUnitScript.h"

class CMonsterATKScript : public CScript
{
protected:
    UnitHit m_HitInfo;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    virtual CMonsterATKScript* Clone() = 0;
    CMonsterATKScript(UINT _Type);
    CMonsterATKScript(const CMonsterATKScript& _Origin);
    virtual ~CMonsterATKScript();
};
