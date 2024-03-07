#pragma once
#include "CComponent.h"

class CCircleCollider2D : public CComponent
{
private:
    void* m_RuntimeFixture;

    Vec2 m_Offset;
    float m_Radius;
    float m_Density;
    float m_Friction;
    float m_Restitution;
    float m_RestitutionThreshold;

public:
    virtual void finaltick() override;

public:
    Vec2 GetOffset() const { return m_Offset; }
    void SetOffset(Vec2 _offset) { m_Offset = _offset; }

    float GetRadius() const { return m_Radius; }
    void SetRadius(float _Radius) { m_Radius = _Radius; }

    float GetDensity() const { return m_Density; }
    void SetDensity(float _Density) { m_Density = _Density; }

    float GetFriction() const { return m_Friction; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }

    float GetRestitution() const { return m_Restitution; }
    void SetRestitution(float _Restitution) { m_Restitution = _Restitution; }

    float GetRestitutionThreshold() const { return m_RestitutionThreshold; }
    void SetRestitutionThreshold(float _RestitutionThreshold) { m_RestitutionThreshold = _RestitutionThreshold; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCircleCollider2D);

public:
    CCircleCollider2D();
    virtual ~CCircleCollider2D();

    friend class CPhysics2DMgr;
};
