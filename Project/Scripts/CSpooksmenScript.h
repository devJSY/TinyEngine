#pragma once
#include "CEnemyScript.h"

enum class SPOOKSMEN_STATE
{
    Hide,
    Appear,
    Idle,
    Trace,
    Attack,
    Uturn,
    Hit1,
    Hit2,
    Surprised,
    Death,
};

class CSpooksmenScript : public CEnemyScript
{
private:
    SPOOKSMEN_STATE m_State;
    int m_AttackCount;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3()) override;
    void ChangeState(SPOOKSMEN_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Hide();
    void Appear();
    void Idle();
    void Trace();
    void Attack();
    void Uturn();
    void Hit1();
    void Hit2();
    void Surprised();
    void Death();

private:
    void OnHitBox(bool _Enable, const wstring& _HitBoxName = L"");

private:
    virtual void OnDetectTargetEnter(CGameObject* _TargetObj) override;
    virtual void OnDetectTargetExit(CGameObject* _TargetObj) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CSpooksmenScript);

public:
    CSpooksmenScript();
    CSpooksmenScript(const CSpooksmenScript& origin);
    virtual ~CSpooksmenScript();
};
