#pragma once
#include "CCollider.h"

class CSphereCollider : public CCollider
{
private:
    float m_Radius;

public:
    virtual void finaltick() override;

public:
    float GetRadius() const { return m_Radius; }
    void SetRadius(float _Radius) { m_Radius = _Radius; };

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CSphereCollider);

public:
    CSphereCollider();
    virtual ~CSphereCollider();

    friend class CPhysicMgr;
};
