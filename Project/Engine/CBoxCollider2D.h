#pragma once
#include "CComponent.h"

class CBoxCollider2D : public CComponent
{
private:
public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBoxCollider2D);

public:
    CBoxCollider2D();
    virtual ~CBoxCollider2D();
};
