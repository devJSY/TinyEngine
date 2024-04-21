#pragma once
#include "CComponent.h"

class CRigidbody : public CComponent
{
private:
public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CRigidbody);

public:
    CRigidbody();
    virtual ~CRigidbody();

    friend class CPhysicMgr;
};
