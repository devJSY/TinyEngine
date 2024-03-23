#pragma once
#include "CCollider2D.h"

class CEdgeCollider2D : public CCollider2D

{
private:
public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CEdgeCollider2D);

public:
    CEdgeCollider2D();
    virtual ~CEdgeCollider2D();

    friend class CPhysics2DMgr;
};
