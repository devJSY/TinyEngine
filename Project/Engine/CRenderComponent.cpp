#include "pch.h"
#include "CRenderComponent.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
    : CComponent(_Type)
    , m_Meshes{}
    , m_Shader(nullptr)
    , m_Material(nullptr)
{
}

CRenderComponent::~CRenderComponent()
{
}
