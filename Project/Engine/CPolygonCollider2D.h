#pragma once
#include "CCollider2D.h"

class CPolygonCollider2D : public CCollider2D
{
private:
public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CPolygonCollider2D);

public:
    CPolygonCollider2D();
    virtual ~CPolygonCollider2D();

    friend class CPhysics2DMgr;
};
