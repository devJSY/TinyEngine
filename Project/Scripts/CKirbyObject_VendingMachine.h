#pragma once
#include "CKirbyObject.h"
class CKirbyObject_VendingMachine : public CKirbyObject
{
private:
    Ptr<CPrefab> m_Can;

    UINT m_LeftCanCount;
    UINT GetCanCount() const { return m_LeftCanCount; }

    float m_SaveJumpPower;

public:
    void ResetCanCount() { m_LeftCanCount = 100; }

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
    CLONE_DISABLE(CKirbyObject_VendingMachine)
    CKirbyObject_VendingMachine();
    virtual ~CKirbyObject_VendingMachine();
};
