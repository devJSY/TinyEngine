#pragma once
#include "CMonsterATKScript.h"
class CAttackAreaScript : public CMonsterATKScript
{
public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void OnTriggerStay(CCollider* _OtherCollider);

public:
    CLONE(CAttackAreaScript);
    CAttackAreaScript();
    virtual ~CAttackAreaScript();
};
