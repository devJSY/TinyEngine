#pragma once
#include "CMonsterUnitScript.h"
enum class HOTHEAD_STATE
{
    Idle,
    Find,
    AttackShotStart,
    AttackShot,
    AttackShotEnd,
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
    void AttackShotStart();
    void AttackShot();
    void AttackShotEnd();
    void AttackFlameStart();
    void AttackFlame();
    void AttackFlameEnd();
    void AttackFlameRotStart();
    void AttackFlameRot();
    void AttackFlameRotEnd();
    void Damage();
    void Death();

public:
    CLONE(CHotHeadScript)
    CHotHeadScript();
    CHotHeadScript(const CHotHeadScript& _Origin);
    virtual ~CHotHeadScript();
};
