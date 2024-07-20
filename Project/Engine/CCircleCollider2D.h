#pragma once
#include "CCollider2D.h"

class CCircleCollider2D : public CCollider2D
{
private:
    float m_Radius;

public:
    virtual void begin() override;
    virtual void finaltick() override;

public:
    float GetRadius() const { return m_Radius; }
    void SetRadius(float _Radius);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CCircleCollider2D);

public:
    CCircleCollider2D();
    virtual ~CCircleCollider2D();

    friend class CPhysics2DMgr;
};
