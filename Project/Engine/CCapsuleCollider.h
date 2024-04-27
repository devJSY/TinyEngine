#pragma once
#include "CCollider.h"

class CCapsuleCollider : public CCollider
{
private:
    float m_Radius;
    float m_Height;
    AXIS_TYPE m_Direction;

public:
    virtual void begin() override;
    virtual void finaltick() override;

public:
    float GetRadius() const { return m_Radius; }
    void SetRadius(float _Radius);

    float GetHeight() const { return m_Height; }
    void SetHeight(float _Height);

    AXIS_TYPE GetAxisDirection() const { return m_Direction; }
    void SetAxisDirection(AXIS_TYPE _Dir);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCapsuleCollider);

public:
    CCapsuleCollider();
    virtual ~CCapsuleCollider();

    friend class CPhysicsMgr;
};
