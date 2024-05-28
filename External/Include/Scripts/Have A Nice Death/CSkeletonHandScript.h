#pragma once
#include "CEnemyScript.h"

enum class SKELETONHAND_STATE
{
    Hide,
    Appear,
    Idle,
    Attack,
    Death
};

class CSkeletonHandScript : public CEnemyScript
{
private:
    SKELETONHAND_STATE m_State;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual bool TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3()) override;
    void ChangeState(SKELETONHAND_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Hide();
    void Appear();
    void Idle();
    void Attack();
    void Death();

private:
    virtual void OnDetectTargetEnter(CGameObject* _TargetObj) override;
    virtual void OnDetectTargetExit(CGameObject* _TargetObj) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CSkeletonHandScript);

public:
    CSkeletonHandScript();
    CSkeletonHandScript(const CSkeletonHandScript& origin);
    virtual ~CSkeletonHandScript();
};
