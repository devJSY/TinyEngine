#include "pch.h"
#include "pch.h"
#include "CRigidbody2D.h"
#include <box2d\\b2_body.h>

CRigidbody2D::CRigidbody2D()
    : CComponent(COMPONENT_TYPE::RIGIDBODY2D)
    , m_RuntimeBody(nullptr)
    , m_BodyType(BODY_TYPE::Dynamic)
    , m_bSimulated(true)
    , m_LinearDrag(0.f)
    , m_AngularDrag(0.05f)
    , m_GravityScale(1.f)
    , m_bFreezeRotation(false)
{
}

CRigidbody2D::~CRigidbody2D()
{
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

void CRigidbody2D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_BodyType, sizeof(BODY_TYPE), 1, _File);
    fwrite(&m_bSimulated, sizeof(bool), 1, _File);
    fwrite(&m_LinearDrag, sizeof(float), 1, _File);
    fwrite(&m_AngularDrag, sizeof(float), 1, _File);
    fwrite(&m_GravityScale, sizeof(float), 1, _File);
    fwrite(&m_bFreezeRotation, sizeof(bool), 1, _File);
}

void CRigidbody2D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_BodyType, sizeof(BODY_TYPE), 1, _File);
    fread(&m_bSimulated, sizeof(bool), 1, _File);
    fread(&m_LinearDrag, sizeof(float), 1, _File);
    fread(&m_AngularDrag, sizeof(float), 1, _File);
    fread(&m_GravityScale, sizeof(float), 1, _File);
    fread(&m_bFreezeRotation, sizeof(bool), 1, _File);
}
