#pragma once
#include "CMonsterUnitScript.h"
enum class BOOMERANGKNIGHT_STATE
{
    Idle,
    Find,
    AirCutterJumpStart,
    AirCutterJump,
    AirCutterThrow,
    CutterThrowStart,
    CutterThrowStartWait,
    CutterThrow,
    CutterCatch,
    Land,
    Damage,
    Eaten,
    Death,
    End,
};

class CBoomerangKnight : public CMonsterUnitScript
{
private:
    BOOMERANGKNIGHT_STATE m_eState;
    Vec3 m_vDamageDir;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void EnterState(BOOMERANGKNIGHT_STATE _state);
    void ChangeState(BOOMERANGKNIGHT_STATE _state);
    void ExitState(BOOMERANGKNIGHT_STATE _state);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

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
    void Death();
    void Land();

public:
    CLONE(CBoomerangKnight)
    CBoomerangKnight();
    CBoomerangKnight(const CBoomerangKnight& _Origin);
    virtual ~CBoomerangKnight();
};