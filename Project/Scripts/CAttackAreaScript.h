#pragma once
#include "CMonsterATKScript.h"
class CAttackAreaScript : public CMonsterATKScript
{
public:
    virtual void begin() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    CLONE(CAttackAreaScript);
    CAttackAreaScript();
    virtual ~CAttackAreaScript();
};
