#pragma once
#include "CKirbyObject.h"
class CKirbyObject_VendingMachine : public CKirbyObject
{
private:
    Ptr<CPrefab> m_Can;
    float m_SaveJumpPower;

public:
    virtual void AttackStart();
    virtual void AttackStartEnter();
    virtual void AttackStartExit();

    virtual void AttackEnd();
    virtual void AttackEndEnter();
    virtual void AttackEndExit();

    // Change/Drop Object
    virtual void DropObjectEnter() override;
    virtual void ChangeObjectEnter() override;

public:
    CLONE(CKirbyObject_VendingMachine)
    CKirbyObject_VendingMachine();
    CKirbyObject_VendingMachine(const CKirbyObject_VendingMachine& _Origin);
    virtual ~CKirbyObject_VendingMachine();
};
