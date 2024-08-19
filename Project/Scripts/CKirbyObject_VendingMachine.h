#pragma once
#include "CKirbyObject.h"
class CKirbyObject_VendingMachine : public CKirbyObject
{
private:
    Ptr<CPrefab> m_Can;
    float m_SaveJumpPower;

public:
    virtual void AttackStart() override;
    virtual void AttackStartEnter() override;
    virtual void AttackStartExit() override;

    virtual void AttackCharge1() override;
    virtual void AttackCharge1Enter() override;
    virtual void AttackCharge1Exit() override;

    virtual void AttackEnd() override;
    virtual void AttackEndEnter() override;
    virtual void AttackEndExit() override;

    // Change/Drop Object
    virtual void DropObjectEnter() override;
    virtual void ChangeObjectEnter() override;

public:
    CLONE(CKirbyObject_VendingMachine)
    CKirbyObject_VendingMachine();
    CKirbyObject_VendingMachine(const CKirbyObject_VendingMachine& _Origin);
    virtual ~CKirbyObject_VendingMachine();
};
