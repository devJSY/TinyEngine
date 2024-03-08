#pragma once
#include "CComponent.h"

class CCollider2D : public CComponent
{
protected:
    void* m_RuntimeFixture;

    Vec2 m_Offset;
    float m_Density;
    float m_Friction;
    float m_Bounciness;

public:
    Vec2 GetOffset() const { return m_Offset; }
    void SetOffset(Vec2 _offset) { m_Offset = _offset; }

    float GetDensity() const { return m_Density; }
    void SetDensity(float _Density) { m_Density = _Density; }

    float GetFriction() const { return m_Friction; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }

    float GetBounciness() const { return m_Bounciness; }
    void SetBounciness(float _Bounciness) { m_Bounciness = _Bounciness; }
      
public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual CCollider2D* Clone() = 0;

public:
    CCollider2D(COMPONENT_TYPE _Type);
    virtual ~CCollider2D();
};
