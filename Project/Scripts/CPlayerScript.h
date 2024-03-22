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
    Hit02,
    Fight_To_Idle,

    // The Scythe
    ComboMove,
    ComboStand,
    Special2,
    Crush,
};

class CPlayerScript : public CScript
{
private:
    PLAYER_STATE m_State;
    DIRECTION_TYPE m_Dir;

    int m_MaxLife;
    int m_MaxMana;

    int m_CurLife;
    int m_CurMana;

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
    int m_AttackCount;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    PLAYER_STATE GetState() const { return m_State; }
    int GetMaxLife() const { return m_MaxLife; }
    int GetMaxMana() const { return m_MaxMana; }
    int GetCurLife() const { return m_CurLife; }
    int GetCurMana() const { return m_CurMana; }

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
    void Hit02();
    void Fight_To_Idle();

    // The Scythe
    void ComboMove();
    void ComboStand();
    void Special2();
    void Crush();

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
