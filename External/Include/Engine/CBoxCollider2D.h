#pragma once
#pragma once
#include "CCollider2D.h"

class CBoxCollider2D : public CCollider2D
{
private:
    Vec2 m_Size;

public:
    virtual void finaltick() override;

public:
    Vec2 GetSize() const { return m_Size; }
    void SetSize(Vec2 _size);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBoxCollider2D);

public:
    CBoxCollider2D();
    virtual ~CBoxCollider2D();

    friend class CPhysics2DMgr;
};
