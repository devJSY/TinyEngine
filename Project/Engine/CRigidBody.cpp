#include "pch.h"
#include "CRigidBody.h"

CRigidbody::CRigidbody()
    : CComponent(COMPONENT_TYPE::RIGIDBODY)
{
}

CRigidbody::~CRigidbody()
{
}

void CRigidbody::finaltick()
{
}

void CRigidbody::SaveToLevelFile(FILE* _File)
{
}

void CRigidbody::LoadFromLevelFile(FILE* _File)
{
}
