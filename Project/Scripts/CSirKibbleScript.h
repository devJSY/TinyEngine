#pragma once
#include "CMonsterUnitScript.h"
enum class SirKibbleState
{
    Idle,
    Patrol,
    PatrolRotating,
    Find,
    FindWait,
    AirCutterJumpStart,
    AirCutterJump,
    AirCutterThrow,
    CutterThrowStart,
    CutterThrowStartWait,
    CutterThrow,
    CutterCatch,
    Fall,
    Land,
    Damage,
    Eaten,
    Death,
    End,
};

class CSirKibbleScript : public CMonsterUnitScript
{
private:
    CGameObject* m_pAttackPoint;
    Vec3 m_vDamageDir;
    Vec3 m_vOriginPos;
    Vec3 m_vDestPos;

    SirKibbleState m_eState;
    float m_fAccTime;
    bool m_bFlag;
    bool m_bJump;
    bool m_bPatrol;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void EnterState(SirKibbleState _state);
    void ChangeState(SirKibbleState _state);
    void ExitState(SirKibbleState _state);
    void LinearMove();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

private:
    void FSM();
    void CheckDamage();

    void Idle();
    void Patrol();
    void PatrolRotating();
    void FindWait();
    void Find();
    void AirCutterJumpStart();
    void AirCutterJump();
    void AirCutterThrow();
    void CutterThrowStart();
    void CutterThrowStartWait();
    void CutterThrow();
    void CutterCatch();
    void Damage();
    void Eaten();
    void Fall();
    void Land();
    void Death();

private:
    void ProjectileAttack(bool _bFlag);

public:
    CLONE(CSirKibbleScript)
    CSirKibbleScript();
    CSirKibbleScript(const CSirKibbleScript& Origin);
    virtual ~CSirKibbleScript();

    friend class CCutterProjectileScript;
};