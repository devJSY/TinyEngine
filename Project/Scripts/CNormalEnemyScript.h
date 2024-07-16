#pragma once
#include <Engine/CScript.h>

#include "CMonsterUnitScript.h"

enum class NORMALENEMY_STATE
{
    Idle,
    Sleep,
    Grooming,
    Patrol,
    Find,
    Attack,
    AttackSuccessed,
    AttackFailed,
    AfterAttack,
    Damage,
    Land,
    Dead,
    End,
};

enum class PATROLDIR
{
    Up,
    Down,
    Right,
    Left,

    UpLeft,
    UpRight,
    DownLeft,
    DownRight,

    END,
};

class CNormalEnemyScript : public CMonsterUnitScript
{
private:
    CGameObject* m_pTargetObject;
    NORMALENEMY_STATE m_eState;
    float m_fPatrolTime;
    float m_fPatrolAccTime;
    Vec3 m_fPatrolDir;

    float m_fMaxSpeed;
    float m_fSpeed;
    float m_fRushLerp;
    float m_fRushSpeedLerp;

protected:
    virtual void TakeHit(const UnitHit& _info, const bool _IsDamaged, const bool _IsHitPlayerBody, const Vec3 _vDamageDir) override;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

private:
    void ChangeState(NORMALENEMY_STATE _state);
    void EnterState(NORMALENEMY_STATE _state);
    void ExitState(NORMALENEMY_STATE _state);

private:
    void Idle();
    void Patrol();
    void Find();
    void Attack();
    void Sleep();
    void Grooming();
    void SuccessedAttack();
    void FailedAttack();
    void Land();
    void AfterAttack();
    void Damage();
    void Dead();

private:
    NORMALENEMY_STATE RandomIdleState();
    Vec3 RandomPatrolDir();
    Vec3 TrackDir(Vec3 _vPos);
    void ApplyDir(Vec3 _vFront, bool _flag);
    void PatrolMove();

public:
    CLONE(CNormalEnemyScript)
    CNormalEnemyScript();
    virtual ~CNormalEnemyScript();
};
