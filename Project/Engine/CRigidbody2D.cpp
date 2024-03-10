#include "pch.h"
#include "pch.h"
#include "CPhysics2DMgr.h"

#include "CRigidbody2D.h"
#include <box2d\\b2_body.h>

CRigidbody2D::CRigidbody2D()
    : CComponent(COMPONENT_TYPE::RIGIDBODY2D)
    , m_RuntimeBody(nullptr)
    , m_BodyType(BODY_TYPE::Dynamic)
    , m_bSimulated(true)
    , m_bAutoMass(false)
    , m_Mass(1.f)
    , m_LinearDrag(0.f)
    , m_AngularDrag(0.05f)
    , m_GravityScale(1.f)
    , m_bFreezeRotation(false)
{
}

CRigidbody2D::~CRigidbody2D()
{
}

void CRigidbody2D::SetBodyType(BODY_TYPE _type)
{
    m_BodyType = _type;
    GamePlayStatic::Physics2D_Event(GetOwner(), Physics2D_EVENT_TYPE::RESPAWN);
}

void CRigidbody2D::AddForce(Vec2 _Force, ForceMode2D _Mode)
{
    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;

    switch (_Mode)
    {
    case Force:
        body->ApplyForceToCenter(b2Vec2(_Force.x, _Force.y), true);
        break;
    case Impulse:
        body->ApplyLinearImpulseToCenter(b2Vec2(_Force.x, _Force.y), true);
        break;
    }
}

void CRigidbody2D::AddTorque(float _Torque, ForceMode2D _Mode)
{
    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;

    switch (_Mode)
    {
    case Force:
        body->ApplyTorque(_Torque, true);
        break;
    case Impulse:
        body->ApplyAngularImpulse(_Torque, true);
        break;
    }
}

Vec2 CRigidbody2D::GetVelocity()
{
    if (nullptr == m_RuntimeBody)
        return Vec2();

    b2Body* body = (b2Body*)m_RuntimeBody;
    b2Vec2 velocity = body->GetLinearVelocity();

    return Vec2(velocity.x, velocity.y);
}

void CRigidbody2D::SetVelocity(Vec2 _Velocity)
{
    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;
    body->SetLinearVelocity(b2Vec2(_Velocity.x, _Velocity.y));
}

void CRigidbody2D::SetSimulated(bool _bSimulated)
{
    m_bSimulated = _bSimulated;

    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;
    body->SetEnabled(m_bSimulated);
}

void CRigidbody2D::SetAutoMass(bool _Use)
{
    m_bAutoMass = _Use;

    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;
    body->ResetMassData();
}

void CRigidbody2D::SetMass(float _Mass)
{
    m_Mass = _Mass;

    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;
    b2MassData MassData = b2MassData();
    MassData.mass = m_Mass;
    body->SetMassData(&MassData);
}

void CRigidbody2D::SetLinearDrag(float _Drag)
{
    m_LinearDrag = _Drag;

    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;
    body->SetLinearDamping(m_LinearDrag);
}

void CRigidbody2D::SetAngularDrag(float _Drag)
{
    m_AngularDrag = _Drag;

    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;
    body->SetAngularDamping(m_AngularDrag);
}

void CRigidbody2D::SetGravityScale(float _Scale)
{
    m_GravityScale = _Scale;

    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;
    body->SetGravityScale(m_GravityScale);
}

void CRigidbody2D::SetFreezeRotation(bool _bFreeze)
{
    m_bFreezeRotation = _bFreeze;

    if (nullptr == m_RuntimeBody)
        return;

    b2Body* body = (b2Body*)m_RuntimeBody;
    body->SetFixedRotation(m_bFreezeRotation);
}

void CRigidbody2D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_BodyType, sizeof(BODY_TYPE), 1, _File);
    fwrite(&m_bSimulated, sizeof(bool), 1, _File);
    fwrite(&m_bAutoMass, sizeof(bool), 1, _File);
    fwrite(&m_Mass, sizeof(float), 1, _File);
    fwrite(&m_LinearDrag, sizeof(float), 1, _File);
    fwrite(&m_AngularDrag, sizeof(float), 1, _File);
    fwrite(&m_GravityScale, sizeof(float), 1, _File);
    fwrite(&m_bFreezeRotation, sizeof(bool), 1, _File);
}

void CRigidbody2D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_BodyType, sizeof(BODY_TYPE), 1, _File);
    fread(&m_bSimulated, sizeof(bool), 1, _File);
    fread(&m_bAutoMass, sizeof(bool), 1, _File);
    fread(&m_Mass, sizeof(float), 1, _File);
    fread(&m_LinearDrag, sizeof(float), 1, _File);
    fread(&m_AngularDrag, sizeof(float), 1, _File);
    fread(&m_GravityScale, sizeof(float), 1, _File);
    fread(&m_bFreezeRotation, sizeof(bool), 1, _File);
}
