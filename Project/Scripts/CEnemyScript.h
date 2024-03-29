#pragma once
#include <Engine\\CScript.h>

class CEnemyScript : public CScript
{
protected:
    DIRECTION_TYPE m_Dir;

    int m_Life;
    float m_Speed;
    int m_ATK; // °ø°Ý·Â
    float m_AttackRange;

    CGameObject* m_pTarget;

protected:
    virtual void tick() override;

public:
    virtual void TakeHit(int _DamageAmount, CGameObject* _HitObj) = 0;

protected:
    void Walking();
    void StopWalking() { Rigidbody2D()->SetVelocity(Vec2(0.f, Rigidbody2D()->GetVelocity().y)); }

    void RotateTransform();

private:
    virtual void OnDetectTargetEnter(CGameObject* _TargetObj) = 0;
    virtual void OnDetectTargetExit(CGameObject* _TargetObj) = 0;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CEnemyScript* Clone() = 0;

public:
    CEnemyScript(UINT _ScriptType);
    CEnemyScript(const CEnemyScript& origin);
    virtual ~CEnemyScript();

    friend class CEnemyColliderScript;
    friend class CEnemyHitBoxScript;
};
