#pragma once
#include <Engine/CScript.h>

#include "CMonsterUnitScript.h"

enum class NORMALENEMY_STATE
{
    Idle,
    Grooming,
    Patrol,
    Sleep,
    Find,
    Attack,
    AttackSuccessed,
    AttackFailed,
    AfterAttack,
    Damage,
    Land,
    Eaten,
    Dead,
    End,
};

class CNormalEnemyScript : public CMonsterUnitScript
{
private:
    CGameObject* m_pTargetObject;
    NORMALENEMY_STATE m_eState;
    float m_fPatrolTime;
    float m_fPatrolAccTime;

    Vec3 m_fPatrolDir;
    Vec3 m_vDamageDir;

    float m_fMaxSpeed;
    float m_fSpeed;
    float m_fRushLerp;
    float m_fRushSpeedLerp;

    bool m_bFirst;

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

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

public:
    CLONE(CNormalEnemyScript)
    CNormalEnemyScript();
    CNormalEnemyScript(const CNormalEnemyScript& _Origin);
    virtual ~CNormalEnemyScript();
};
