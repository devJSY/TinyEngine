#pragma once
#include "CMonsterUnitScript.h"
enum class HotHeadState
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
    Fall,
    Landing,
    Damage,
    Eaten,
    Death,
    End,
};

class CHotHeadScript : public CMonsterUnitScript
{
private:
    CGameObject* m_pFlameRotObject;

    HotHeadState m_eState;
    Vec3 m_vDamageDir;

    float m_fAimingTime;
    float m_fAccTime;

    float m_fShotLength;
    float m_fFlameLength;
    float m_fFlameRotLength;

    float m_fRotRadian;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void InitSetting();
    void EnterState(HotHeadState _state);
    void FSM();
    void ExitState(HotHeadState _state);
    void ChangeState(HotHeadState _state);
    void CheckDamage();
    void ProjectileAttack();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

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
    void Fall();
    void Landing();
    void Damage();
    void Eaten();
    void Death();

public:
    CLONE(CHotHeadScript)
    CHotHeadScript();
    CHotHeadScript(const CHotHeadScript& _Origin);
    virtual ~CHotHeadScript();
};
