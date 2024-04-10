#pragma once
#pragma once
#include "CBossEnemyScript.h"

enum class LIFE_STATE
{
    Hide,
    Intro,
    SecondPhase,
    ThirdPhase,
    Idle,
    Run,
    Hit,
    Stun,
    Uturn,
    Attack1,
    Attack2,
    Attack3,
    Attack4,
    Attack5,
    Skill1,
    Skill2,
    Death,
};

class CLifeScript : public CBossEnemyScript
{
private:
    LIFE_STATE m_State;
    float m_PassedTime;

    bool m_bAttackStart;
    bool m_bAttackEnd;

    float TestForce;

    Ptr<CPrefab> m_pFeatherProjPref;

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
    void SecondPhase();
    void ThirdPhase();
    void Idle();
    void Run();
    void Hit();
    void Stun();
    void Uturn();
    void Attack1();
    void Attack2();
    void Attack3();
    void Attack4();
    void Attack5();
    void Skill1();
    void Skill2();
    void Death();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLifeScript);

public:
    CLifeScript();
    virtual ~CLifeScript();
};
