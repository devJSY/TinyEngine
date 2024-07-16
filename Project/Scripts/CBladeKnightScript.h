#pragma once
#include "CMonsterUnitScript.h"

class CBladeKnightScript : public CMonsterUnitScript
{
private:
private:
    virtual void OnCollisionEnter(CCollider* _OtherCollider);
    virtual void OnCollisionStay(CCollider* _OtherCollider);
    virtual void OnCollisionExit(CCollider* _OtherCollider);

    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);
    virtual void OnTriggerExit(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBladeKnightScript);

public:
    CBladeKnightScript();
    CBladeKnightScript(const CBladeKnightScript& origin);
    virtual ~CBladeKnightScript();
};
