#include "pch.h"
#include "CRenderComponent.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_Mesh(nullptr)
    , m_Material(nullptr)
{
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SaveToLevelFile(FILE* _File)
{
}

void CRenderComponent::LoadFromLevelFile(FILE* _File)
{
}
