#pragma once
#include "CMonsterUnitScript.h"
enum class SIRKIBBLE_STATE
{
    Idle,
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
    CGameObject* m_pTargetObj;
    CGameObject* m_pAttackPoint;
    SIRKIBBLE_STATE m_eState;
    Vec3 m_vDamageDir;

    float m_fAccTime;
    bool m_bFlag;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void EnterState(SIRKIBBLE_STATE _state);
    void ChangeState(SIRKIBBLE_STATE _state);
    void ExitState(SIRKIBBLE_STATE _state);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

private:
    void Idle();
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
    void ProjectileAttack();

public:
    CLONE(CSirKibbleScript)
    CSirKibbleScript();
    CSirKibbleScript(const CSirKibbleScript& _Origin);
    virtual ~CSirKibbleScript();

    friend class CCutterProjectileScript;
};