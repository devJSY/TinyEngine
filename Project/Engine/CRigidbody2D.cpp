#include "pch.h"
#include "CRigidbody2D.h"

CRigidbody2D::CRigidbody2D()
    : CComponent(COMPONENT_TYPE::RIGIDBODY2D)
{
}

CRigidbody2D::~CRigidbody2D()
{
}

void CRigidbody2D::finaltick()
{
}

void CRigidbody2D::SaveToLevelFile(FILE* _File)
{
}

void CRigidbody2D::LoadFromLevelFile(FILE* _File)
{
}
