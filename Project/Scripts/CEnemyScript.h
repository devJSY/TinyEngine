#pragma once
#include <Engine\\CScript.h>
#include "CEnemyHitBoxScript.h"
#include <Engine\\CAnim.h>

class CEnemyScript : public CScript
{
protected:
    DIRECTION_TYPE m_Dir;

    int m_MaxLife;
    int m_CurLife;
    float m_Speed;
    int m_ATK; // °ø°Ý·Â
    float m_AttackRange;

    CGameObject* m_pTarget;

protected:
    virtual void tick() override;

public:
    int GetMaxLife() const { return m_MaxLife; }
    int GetCurLife() const { return m_CurLife; }

public:
    virtual bool TakeHit(int _DamageAmount, Vec3 _Hitdir = Vec3()) = 0;

protected:
    void Walking();
    void StopWalking() { Rigidbody2D()->SetVelocity(Vec2(0.f, Rigidbody2D()->GetVelocity().y)); }
    void StopMoving() { Rigidbody2D()->SetVelocity(Vec2(0.f, 0.f)); }

    void RotateTransform();

    void SetHitBox(bool _Enable, const wstring& _HitBoxName = L"");

protected:
    virtual void OnDetectTargetEnter(CGameObject* _TargetObj) { m_pTarget = _TargetObj; };
    virtual void OnDetectTargetExit(CGameObject* _TargetObj) { m_pTarget = nullptr; };

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CEnemyScript* Clone() = 0;

public:
    CEnemyScript(UINT _ScriptType);
    CEnemyScript(const CEnemyScript& origin);
    virtual ~CEnemyScript();

    friend class CPlayerDetectScript;
    friend class CEnemyHitBoxScript;
};
