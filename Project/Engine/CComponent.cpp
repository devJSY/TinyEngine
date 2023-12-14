#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _Type)
    : m_Type(_Type)
    , m_Owner(nullptr)
{
}

CComponent::~CComponent()
{
}
