#pragma once

#include <Engine/CScript.h>

#define DOT_TERM 1.f

enum class DAMAGE_TYPE
{
    NORMAL,
    DOT,

    END,
};

// Unit ���� ����ü�� �ڽ� �����ڿ��� �־��ֱ�
struct UnitInfo
{
    float HP;
    float Speed;
    float JumpPower;
    float MAXHP;
};

struct UnitHit
{
    DAMAGE_TYPE Type;
    float       Damage;
    float       Duration;
    float       Acc;
};

class CUnitScript : public CScript
{
private:
    UnitInfo       m_CurInfo;
    UnitInfo       m_PrevInfo;
    std::list<UnitHit> m_HitHistory;

public:
    virtual void tick() override;
    virtual void AttackReward() {}              // Atk: Unit�� Attack�� ���� ���� ó�� (����ȿ�� ��)
    virtual void GetDamage(UnitHit _Damage);    // Hit: Unit�� Hit�� ���� �г�Ƽ ó�� (HP���� ��)

private:
    void DamageProc();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CUnitScript* Clone() = 0;
public: 
    CUnitScript(SCRIPT_TYPE _Type);
    virtual ~CUnitScript();
};