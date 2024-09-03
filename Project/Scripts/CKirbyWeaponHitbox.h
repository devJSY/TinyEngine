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
    void ApplyEffect();

private:
    void SlashEffect(Vec3 _vPos);
    void SpawnSlashEffect(Vec3 _vPos, int _iCount);

public:
    CLONE(CKirbyWeaponHitbox)
    CKirbyWeaponHitbox();
    virtual ~CKirbyWeaponHitbox();
};