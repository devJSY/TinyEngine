#include "pch.h"
#include "CLight3D.h"
#include "CTransform.h"

CLight3D::CLight3D(LIGHT_TYPE type)
    : CComponent(COMPONENT_TYPE::LIGHT3D)
    , m_Type(type)
{
    m_LightData.fallOffStart = 0.f;
    m_LightData.fallOffEnd = 10000.f;
    m_LightData.spotPower = 100.f;
    m_LightData.strength = Vec3(1.f);
}

CLight3D::~CLight3D()
{
}

void CLight3D::finaltick()
{
    m_LightData.position = GetOwner()->Transform()->GetRelativePos();
    m_LightData.direction = GetOwner()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

    if (m_Type == LIGHT_TYPE::DIRECTIONAL)
    {
        g_Global.DirLight = m_LightData;

        g_Global.PointLight.strength *= 0.f;
        g_Global.SpotLight.strength *= 0.f;
    }
    else if (m_Type == LIGHT_TYPE::POINT)
    {
        g_Global.PointLight = m_LightData;

        g_Global.DirLight.strength *= 0.f;
        g_Global.SpotLight.strength *= 0.f;
    }
    else if (m_Type == LIGHT_TYPE::SPOT)
    {
        g_Global.SpotLight = m_LightData;

        g_Global.DirLight.strength *= 0.f;
        g_Global.PointLight.strength *= 0.f;
    }
}
