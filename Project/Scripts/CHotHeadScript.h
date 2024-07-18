#pragma once
#include "CMonsterUnitScript.h"
enum class HOTHEAD_STATE
{
    Idle,
    Find,
    Aiming,
    AttackShootStart,
    AttackShoot,
    AttackShootEnd,
    AttackFlameStart,
    AttackFlame,
    AttackFlameEnd,
    AttackFlameRotStart,
    AttackFlameRot,
    AttackFlameRotEnd,
    Damage,
    Eaten,
    Death,
    End,
};

class CHotHeadScript : public CMonsterUnitScript
{
private:
    HOTHEAD_STATE m_eState;
    Vec3 m_vDamageDir;

    float m_fAimingTime;
    float m_fAccTime;

    float m_fShotLength;
    float m_fFlameLength;
    float m_fFlameRotLength;

    bool m_bFlag;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void EnterState(HOTHEAD_STATE _state);
    void ChangeState(HOTHEAD_STATE _state);
    void ExitState(HOTHEAD_STATE _state);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

private:
    void Idle();
    void Find();
    void Aiming();
    void AttackShootStart();
    void AttackShoot();
    void AttackFlameStart();
    void AttackFlame();
    void AttackFlameRotStart();
    void AttackFlameRot();
    void AttackEnd();
    void Damage();
    void Eaten();
    void Death();

public:
    CLONE(CHotHeadScript)
    CHotHeadScript();
    CHotHeadScript(const CHotHeadScript& _Origin);
    virtual ~CHotHeadScript();
};
