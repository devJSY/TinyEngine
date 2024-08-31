#pragma once
#include "CUnitScript.h"

class CBossHitbox : public CScript
{
private:
    CGameObject* m_Instigator;
    CGameObject* m_Target;
    CCollider* m_Collider;
    bool m_bSummon;
    bool m_bCallReward;

    // damage
    float m_RandMin;
    float m_RandMax;
    int m_DamageTypeIdx;

    // repeat
    float m_AccTime;
    float m_RepeatTime;
    bool m_bRepeatDamage;
    bool m_bRepeat;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);
    virtual void OnTriggerStay(CCollider* _OtherCollider);

    float GetRandDamage();
    void AddDamage();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CBossHitbox)
    CBossHitbox();
    CBossHitbox(const CBossHitbox& _Origin);
    virtual ~CBossHitbox();
};