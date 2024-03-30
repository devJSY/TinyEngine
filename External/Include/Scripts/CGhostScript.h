#pragma once
#include "CEnemyScript.h"

enum class GHOST_STATE
{
    Waiting,
    Appear,
    Idle,
    Run,
    Trace,
    Attack,
    Uturn,
    Hit1,
    Hit2,
    Death,
};

class CGhostScript : public CEnemyScript
{
private:
    GHOST_STATE m_State;

    float m_PassedTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3()) override;
    void ChangeState(GHOST_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Waiting();
    void Appear();
    void Idle();
    void Run();
    void Trace();
    void Attack();
    void Uturn();
    void Hit1();
    void Hit2();
    void Death();

private:
    virtual void OnDetectTargetEnter(CGameObject* _TargetObj) override;
    virtual void OnDetectTargetExit(CGameObject* _TargetObj) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CGhostScript);

public:
    CGhostScript();
    CGhostScript(const CGhostScript& origin);
    virtual ~CGhostScript();
};
