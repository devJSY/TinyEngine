#pragma once
#include "CUnitScript.h"

class CPlayerHitbox : public CScript
{
private:
    CGameObject* m_Instigator;
    CCollider* m_Collider;
    bool m_bSummon;
    bool m_bCallReward;

    // damage
    float m_Damage;
    int m_DamageTypeIdx;

    // repeat
    float m_AccTime;
    float m_RepeatTime;
    bool m_bRepeatDamage;
    bool m_bRepeatEnter;
    bool m_bRepeat;

    // cooltime
    float m_CoolTime;
    float m_PrevAttackTime;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    void AddDamage(CGameObject* _Monster);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CPlayerHitbox)
    CPlayerHitbox();
    CPlayerHitbox(const CPlayerHitbox& _Origin);
    virtual ~CPlayerHitbox();
};