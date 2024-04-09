#pragma once
#include "CEnemyScript.h"

enum class FLYINGBOOK_STATE
{
    Idle,
    Attack,
    Uturn,
    Hit,
    Death
};

class CFlyingBookScript : public CEnemyScript
{
private:
    FLYINGBOOK_STATE m_State;
    float m_PassedTime;
    float m_PatrolDuration;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual bool TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3()) override;
    void ChangeState(FLYINGBOOK_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Idle();
    void Attack();
    void Uturn();
    void Hit();
    void Death();

private:
    virtual void OnDetectTargetEnter(CGameObject* _TargetObj) override;
    virtual void OnDetectTargetExit(CGameObject* _TargetObj) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CFlyingBookScript);

public:
    CFlyingBookScript();
    CFlyingBookScript(const CFlyingBookScript& origin);
    virtual ~CFlyingBookScript();
};
