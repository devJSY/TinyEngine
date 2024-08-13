#pragma once
#include "CKirbyObject.h"

class CKirbyObject_Lightbulb : public CKirbyObject
{
private:
    CGameObject* m_PointLight;
    float m_Speed;
    float m_BrightSpeed;

public:
    // 기본 상태들 (Idle, Run, Jump)
    virtual void IdleEnter();
    virtual void IdleExit();

    virtual void RunEnter();

    virtual void RunEndEnter();
    virtual void RunEndExit();

    virtual void JumpStartEnter();

    virtual void LandingEnter();
    virtual void LandingExit();

    virtual void LandingEndEnter();
    virtual void LandingEndExit();

    // 불켜기
    virtual void Attack();
    virtual void AttackEnter();
    virtual void AttackExit();

    virtual void AttackEnd();
    virtual void AttackEndEnter();
    virtual void AttackEndExit();

    // Change/Drop Object
    virtual void DropObjectEnter();
    virtual void ChangeObjectEnter();

public:
    CLONE(CKirbyObject_Lightbulb)
    CKirbyObject_Lightbulb();
    CKirbyObject_Lightbulb(const CKirbyObject_Lightbulb& _Origin);
    virtual ~CKirbyObject_Lightbulb();
};
