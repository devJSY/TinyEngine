#pragma once
#include "CCollider.h"

class CBoxCollider : public CCollider
{
private:
    Vec3 m_Size;

public:
    virtual void begin() override;
    virtual void finaltick() override;

public:
    Vec3 GetSize() const { return m_Size; }
    void SetSize(Vec3 _size);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBoxCollider);

public:
    CBoxCollider();
    virtual ~CBoxCollider();

    friend class CPhysicsMgr;
};
