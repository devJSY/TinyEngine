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
    Hit01,
    Hit02,
    Fight_To_Idle,
    Elevator_In,
    Elevator_End,
    Elevator_Out,
    Cinematic,

    // The Scythe
    ComboMove,
    ComboAerial,
    JumpAttack,
    DownAttack,
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
    int m_ATK;
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
    Ptr<CPrefab> m_pShockWavePref;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    PLAYER_STATE GetState() const { return m_State; }
    int GetMaxLife() const { return m_MaxLife; }
    int GetMaxMana() const { return m_MaxMana; }
    int GetCurLife() const { return m_CurLife; }
    int GetCurMana() const { return m_CurMana; }

public:
    void ChangeState(PLAYER_STATE _NextState);
    bool TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3());

private:
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
    void Hit01();
    void Hit02();
    void Fight_To_Idle();
    void Elevator_In();
    void Elevator_End();
    void Elevator_Out();
    void Cinematic();

    // The Scythe
    void ComboMove();
    void ComboAerial();
    void JumpAttack();
    void DownAttack();

private:
    void Walking();
    void StopWalking() { Rigidbody2D()->SetVelocity(Vec2(0.f, Rigidbody2D()->GetVelocity().y)); }

    void RotateTransform();

    void GroundCheck();

    void ChangeStateToJump();

    void SetHitBox(bool _Enable, const wstring& _HitBoxName = L"");

    void EnableParticle(bool _bEnable);

private:
    virtual void OnCollisionEnter(CCollider2D* _OtherCollider) override;
    virtual void OnCollisionStay(CCollider2D* _OtherCollider) override;
    virtual void OnCollisionExit(CCollider2D* _OtherCollider) override;

    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;
    virtual void OnTriggerStay(CCollider2D* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPlayerScript);

public:
    CPlayerScript();
    CPlayerScript(const CPlayerScript& origin);
    virtual ~CPlayerScript();

    friend class CPlayerHitBoxScript;
};
