#pragma once
#include "CEnemyScript.h"

enum class PYROGHOST_STATE
{
    Idle,
    Hide,
    Appear,
    Trace,
    Attack,
    Uturn,
    Spotted,
    Hit1,
    Hit2,
    Death,
};

class CPyroGhostScript : public CEnemyScript
{
private:
    PYROGHOST_STATE m_State;

    float m_PassedTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void ChangeState(PYROGHOST_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Idle();
    void Hide();
    void Appear();
    void Trace();
    void Attack();
    void Uturn();
    void Spotted();
    void Hit1();
    void Hit2();
    void Death();

private:
    virtual void OnDetectTargetEnter(CGameObject* _TargetObj) override;
    virtual void OnDetectTargetExit(CGameObject* _TargetObj) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPyroGhostScript);

public:
    CPyroGhostScript();
    CPyroGhostScript(const CPyroGhostScript& origin);
    virtual ~CPyroGhostScript();
};
