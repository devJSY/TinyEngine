#pragma once
#include <Engine\\CScript.h>

class CKirbyHatBlade : public CScript
{
private:
    float       m_AttackCoolTime;
    float       m_Acc;
    bool        m_bCanAttack;

    float       m_VariationDuration;
    float       m_VariationAcc;

public:
    // 커비가 공격 했을 경우
    void Attack();

    // Blade가 다시 머리에 돌아왔을 경우 공격 가능한 상태로 초기화
    void Reset();

    // 공격 가능한 상태인지
    bool CanAttack() { return m_bCanAttack; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override{};
    virtual UINT LoadFromLevelFile(FILE* _File) override{};

public:
    CLONE(CKirbyHatBlade);
    CKirbyHatBlade();
    virtual ~CKirbyHatBlade();

};
