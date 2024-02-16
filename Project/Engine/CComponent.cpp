#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _Type)
    : m_Type(_Type)
    , m_Owner(nullptr)
{
}

CComponent::CComponent(const CComponent& origin)
    : CEntity(origin)
    , m_Type(origin.m_Type)
    , m_Owner(nullptr)
{
}

CComponent::~CComponent()
{
}
