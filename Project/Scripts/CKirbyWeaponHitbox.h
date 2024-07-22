#pragma once
#include "CUnitScript.h"

class CKirbyWeaponHitbox : public CScript
{
private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    float LoadDamage();

public:
    CLONE(CKirbyWeaponHitbox)
    CKirbyWeaponHitbox();
    virtual ~CKirbyWeaponHitbox();
};