#pragma once
#include <Engine\\CScript.h>

enum class PYROGHOST_STATE
{
    Idle,
    Hide,
    Appear,
    Trace,
    Attack,
    Uturn,
    Spotted,
    Hit1,
    Hit2,
    Death,
};

class CPyroGhostScript : public CScript
{
private:
    PYROGHOST_STATE m_State;
    DIRECTION_TYPE m_Dir;

    int m_Life;
    float m_Speed;
    float m_ATK; // °ø°Ý·Â
    float m_AttackRange; 
    float m_PassedTime;

    CGameObject* m_pTarget;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void ChangeState(PYROGHOST_STATE _NextState);

private:
    void EnterState();
    void ExitState();

    void Idle();
    void Hide();
    void Appear();
    void Trace();
    void Attack();
    void Uturn();
    void Spotted();
    void Hit1();
    void Hit2();
    void Death();

private:
    void Walking();
    void StopWalking() { Rigidbody2D()->SetVelocity(Vec2(0.f, Rigidbody2D()->GetVelocity().y)); }

    void RotateTransform();

public:
    virtual void OnCollisionEnter(CCollider2D* _OtherCollider) override;
    virtual void OnCollisionStay(CCollider2D* _OtherCollider) override;
    virtual void OnCollisionExit(CCollider2D* _OtherCollider) override;

    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;
    virtual void OnTriggerStay(CCollider2D* _OtherCollider) override;
    virtual void OnTriggerExit(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPyroGhostScript);

public:
    CPyroGhostScript();
    CPyroGhostScript(const CPyroGhostScript& origin);
    virtual ~CPyroGhostScript();
};
