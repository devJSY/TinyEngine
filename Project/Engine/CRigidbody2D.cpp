#include "pch.h"
#include "pch.h"
#include "CRigidbody2D.h"

CRigidbody2D::CRigidbody2D()
    : CComponent(COMPONENT_TYPE::RIGIDBODY2D)
    , m_RuntimeBody(nullptr)
    , m_BodyType(BODY_TYPE::Dynamic)
    , m_bFreezeRotation(false)
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
    fwrite(&m_BodyType, sizeof(BODY_TYPE), 1, _File);
    fwrite(&m_bFreezeRotation, sizeof(bool), 1, _File);
}

void CRigidbody2D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_BodyType, sizeof(BODY_TYPE), 1, _File);
    fread(&m_bFreezeRotation, sizeof(bool), 1, _File);
}
