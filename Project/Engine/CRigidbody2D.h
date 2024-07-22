#pragma once
#include "CComponent.h"

enum class BODY_TYPE
{
    Dynamic = 0,
    Kinematic,
    Static,
};

enum class ForceMode2D
{
    Force,
    Impulse,
};

class CRigidbody2D : public CComponent
{
private:
    void* m_RuntimeBody;
    BODY_TYPE m_BodyType;
    bool m_bSimulated;
    bool m_bAutoMass;
    float m_Mass;
    float m_LinearDrag;
    float m_AngularDrag;
    float m_GravityScale;
    bool m_bFreezeRotation;

public:
    virtual void finaltick() override;

public:
    void AddForce(Vec2 _Force, ForceMode2D _Mode = ForceMode2D::Force);
    void AddTorque(float _Torque, ForceMode2D _Mode = ForceMode2D::Force);
    Vec2 GetVelocity();
    void SetVelocity(Vec2 _Velocity);

public:
    BODY_TYPE GetBodyType() const { return m_BodyType; }
    void SetBodyType(BODY_TYPE _type);

    bool IsSimulated() const { return m_bSimulated; }
    void SetSimulated(bool _bSimulated);

    bool IsUseAutoMass() const { return m_bAutoMass; }
    void SetAutoMass(bool _Use);

    float GetMass() const { return m_Mass; }
    void SetMass(float _Mass);

    float GetLinearDrag() const { return m_LinearDrag; }
    void SetLinearDrag(float _Drag);

    float GetAngularDrag() const { return m_AngularDrag; }
    void SetAngularDrag(float _Drag);

    float GetGravityScale() const { return m_GravityScale; }
    void SetGravityScale(float _Scale);

    bool IsFreezeRotation() const { return m_bFreezeRotation; }
    void SetFreezeRotation(bool _bFreeze);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CRigidbody2D);

public:
    CRigidbody2D();
    CRigidbody2D(const CRigidbody2D& origin);
    virtual ~CRigidbody2D();

    friend class CPhysics2DMgr;
};
