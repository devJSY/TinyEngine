#pragma once
#include <Engine\\CScript.h>

enum class PLAYER_STATE
{
    Idle,
    IdleToRun,
    IdleUturn,
    Jump_Falling,
    Jump_Start,
    Jump_Landing,
    Run,
    RunUturn,
    RunToIdle,
    ComboMove,
    ComboMove_Rest,
    ComboAerial,
    ComboAerial_Rest,
    JumpingAttack,
    AerialDownAttack,
    CapeAttack,
    Dash,
    Hit,
    EnterElavator,
    ExitElavator,
    Acquisition,
    PowerUp,
    UltAttack,
    UltAttack_Rest,
};

class CPlayerScript : public CScript
{
private:
    PLAYER_STATE m_State;
    DIRECTION_TYPE m_Dir;

    float m_Speed;
    float m_JumpImpulse;
    float m_JumpForce;

    bool m_bOnGround;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(PLAYER_STATE _NextState);
    void EnterState();
    void ExitState();

    // FSM
    void Idle();
    void IdleToRun();
    void IdleUturn();
    void Jump_Falling();
    void Jump_Start();
    void Jump_Landing();
    void Run();
    void RunUturn();
    void RunToIdle();
    void ComboMove();
    void ComboMove_Rest();
    void ComboAerial();
    void ComboAerial_Rest();
    void JumpingAttack();
    void AerialDownAttack();
    void CapeAttack();
    void Dash();
    void Hit();
    void EnterElavator();
    void ExitElavator();
    void Acquisition();
    void PowerUp();
    void UltAttack();
    void UltAttack_Rest();

    void MoveTransform();
    void RotateTransform();

private:
    virtual void OnCollisionEnter(CCollider2D* _OtherCollider);
    virtual void OnCollisionStay(CCollider2D* _OtherCollider);
    virtual void OnCollisionExit(CCollider2D* _OtherCollider);

    virtual void OnTriggerEnter(CCollider2D* _OtherCollider);
    virtual void OnTriggerStay(CCollider2D* _OtherCollider);
    virtual void OnTriggerExit(CCollider2D* _OtherCollider);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerScript);

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};
