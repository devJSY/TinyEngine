#pragma once
#include "CBossEnemyScript.h"

enum class EVILPUMPKINSCRIPT_STATE
{
    Hide,
    Intro,
    Idle,
    Run,
    Hit,
    Stun,
    Attack,
    Death,
};

class CEvilPumpkinScript : public CBossEnemyScript
{
private:
    EVILPUMPKINSCRIPT_STATE m_State;
    int m_AttackCount;
    float m_PassedTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual bool TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3()) override;
    void ChangeState(EVILPUMPKINSCRIPT_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Hide();
    void Intro();
    void Idle();
    void Run();
    void Hit();
    void Stun();
    void Attack();
    void Death();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CEvilPumpkinScript);

public:
    CEvilPumpkinScript();
    virtual ~CEvilPumpkinScript();
};
