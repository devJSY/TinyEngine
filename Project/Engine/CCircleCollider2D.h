#pragma once
#include "CComponent.h"

class CCircleCollider2D : public CComponent
{
private:
public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCircleCollider2D);

public:
    CCircleCollider2D();
    virtual ~CCircleCollider2D();
};
