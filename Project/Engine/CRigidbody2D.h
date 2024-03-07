#pragma once
#include "CComponent.h"

class CRigidbody2D : public CComponent
{
private:
public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CRigidbody2D);

public:
    CRigidbody2D();
    virtual ~CRigidbody2D();
};
