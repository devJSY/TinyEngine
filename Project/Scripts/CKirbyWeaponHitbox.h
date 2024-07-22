#pragma once
#include "CUnitScript.h"

class CKirbyWeaponHitbox : public CScript
{
private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    float LoadDamage();

public:
    CLONE(CKirbyWeaponHitbox)
    CKirbyWeaponHitbox();
    virtual ~CKirbyWeaponHitbox();
};