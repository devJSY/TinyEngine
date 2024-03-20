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
    Dash,
    Hit,
    FightToIdle,

    // The Scythe
    ComboMove_01,
    ComboMove_02,
    ComboMove_03,
    ComboMove_04,
    ComboAerial_01,
    ComboAerial_02,
    ComboAerial_03,
    JumpingAttack,
    AerialDownAttack,
};

class CPlayerScript : public CScript
{
private:
    PLAYER_STATE m_State;
    DIRECTION_TYPE m_Dir;

    // 수정 가능한 멤버변수
    float m_Speed;
    float m_JumpImpulse;
    float m_JumpForce;
    float m_DashImpulse;
    float m_AttackImpulse;
    float m_RaycastDist;

    // 수정 불가능한 멤버변수
    bool m_bOnGround;
    float m_DashPassedTime;
    const float m_DashCoolTime;
    float m_RigidGravityScale;
    bool m_bJumpAttackActive;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    PLAYER_STATE GetState() const { return m_State; }

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
    void Dash();
    void Hit();
    void FightToIdle();

    // The Scythe
    void ComboMove_01();
    void ComboMove_02();
    void ComboMove_03();
    void ComboMove_04();
    void ComboAerial_01();
    void ComboAerial_02();
    void ComboAerial_03();
    void JumpingAttack();
    void AerialDownAttack();

private:
    void Walking();
    void StopWalking() { Rigidbody2D()->SetVelocity(Vec2(0.f, Rigidbody2D()->GetVelocity().y)); }

    void RotateTransform();
    void RayCast();

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
