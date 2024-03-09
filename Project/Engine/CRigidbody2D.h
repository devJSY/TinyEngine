#pragma once
#include "CComponent.h"

enum class BODY_TYPE
{
    Dynamic = 0,
    Kinematic,
    Static,
};

class CRigidbody2D : public CComponent
{
private:
    void* m_RuntimeBody;
    BODY_TYPE m_BodyType;
    bool m_bSimulated;
    float m_LinearDrag;
    float m_AngularDrag;
    float m_GravityScale;
    bool m_bFreezeRotation;

public:
    virtual void finaltick() override;

public:
    BODY_TYPE GetBodyType() const { return m_BodyType; }
    void SetBodyType(BODY_TYPE _type) { m_BodyType = _type; }

    bool IsSimulated() const { return m_bSimulated; }
    void SetSimulated(bool _bSimulated) { m_bSimulated = _bSimulated; }

    float GetLinearDrag() const { return m_LinearDrag; }
    void SetLinearDrag(float _drag) { m_LinearDrag = _drag; }

    float GetAngularDrag() const { return m_AngularDrag; }
    void SetAngularDrag(float _drag) { m_AngularDrag = _drag; }

    float GetGravityScale() const { return m_GravityScale; }
    void SetGravityScale(float _scale) { m_GravityScale = _scale; }

    bool IsFreezeRotation() const { return m_bFreezeRotation; }
    void SetFreezeRotation(bool _bFreeze) { m_bFreezeRotation = _bFreeze; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CRigidbody2D);

public:
    CRigidbody2D();
    virtual ~CRigidbody2D();

    friend class CPhysics2DMgr;
};
