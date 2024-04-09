#pragma once
#include "CBossEnemyScript.h"

enum class LIFE_STATE
{
    Hide,
    Intro,
    Idle,
    Run,
    Hit,
    Stun,
    Uturn,
    Attack,
    Death,
};

class CLifeScript : public CBossEnemyScript
{
private:
    LIFE_STATE m_State;
    int m_AttackCount;
    float m_PassedTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual bool TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3()) override;
    void ChangeState(LIFE_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Hide();
    void Intro();
    void Idle();
    void Run();
    void Hit();
    void Stun();
    void Uturn();
    void Attack();
    void Death();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLifeScript);

public:
    CLifeScript();
    virtual ~CLifeScript();
};
