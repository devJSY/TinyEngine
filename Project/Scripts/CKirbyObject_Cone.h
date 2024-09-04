#pragma once
#include "CKirbyObject.h"

class CKirbyObject_Cone : public CKirbyObject
{
private:
    Ptr<CPrefab> m_Crater;
    float m_PrevSpeed;
    float m_PrevJumpPower;
    float m_PrevGravity;
    bool m_bFrmEnter;

public:
    // 내려찍기 (시작)
    virtual void Attack();
    virtual void AttackEnter();
    virtual void AttackExit();

    virtual void AttackStart();
    virtual void AttackStartEnter();
    virtual void AttackStartExit();

    virtual void AttackEnd();
    virtual void AttackEndEnter();
    virtual void AttackEndExit();

    // 내려찍기 (끝)
    virtual void AttackCombo1();
    virtual void AttackCombo1Enter();
    virtual void AttackCombo1Exit();

    virtual void AttackCombo1End();
    virtual void AttackCombo1EndEnter();
    virtual void AttackCombo1EndExit();

    // Change/Drop Object
    virtual void DropObjectEnter();
    virtual void ChangeObjectEnter();

    virtual void LandingEnter() override;

public:
    CLONE(CKirbyObject_Cone)
    CKirbyObject_Cone();
    CKirbyObject_Cone(const CKirbyObject_Cone& _Origin);
    virtual ~CKirbyObject_Cone();
};
