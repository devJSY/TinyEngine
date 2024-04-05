#pragma once
#include "CEnemyScript.h"

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

class CEvilPumpkinScript : public CEnemyScript
{
private:
    EVILPUMPKINSCRIPT_STATE m_State;
    int m_AttackCount;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3()) override;
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

private:
    virtual void OnDetectTargetEnter(CGameObject* _TargetObj) override;
    virtual void OnDetectTargetExit(CGameObject* _TargetObj) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CEvilPumpkinScript);

public:
    CEvilPumpkinScript();
    CEvilPumpkinScript(const CEvilPumpkinScript& origin);
    virtual ~CEvilPumpkinScript();
};
