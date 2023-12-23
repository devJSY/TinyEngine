#include "pch.h"
#include "CLight3D.h"
#include "CTransform.h"

CLight3D::CLight3D(LIGHT_TYPE type)
    : CComponent(COMPONENT_TYPE::LIGHT3D)
    , m_Type(type)
    , m_FallOffStart(0.f)
    , m_FallOffEnd(10000.f)
    , m_SpotPower(100.f)
    , m_Strength(Vec3(1.f))
{
}

CLight3D::~CLight3D()
{
}

void CLight3D::finaltick()
{
    m_Light.position = GetOwner()->Transform()->GetRelativePos();
    m_Light.direction = GetOwner()->Transform()->GetLocalDir(DIR_TYPE::FRONT);
    m_Light.fallOffStart = m_FallOffStart;
    m_Light.fallOffEnd = m_FallOffEnd;
    m_Light.spotPower = m_SpotPower;
    m_Light.strength = m_Strength;

    if (m_Type == LIGHT_TYPE::DIRECTIONAL)
    {
        g_Global.DirLight = m_Light;

        g_Global.PointLight.strength *= 0.f;
        g_Global.SpotLight.strength *= 0.f;
    }
    else if (m_Type == LIGHT_TYPE::POINT)
    {
        g_Global.PointLight = m_Light;

        g_Global.DirLight.strength *= 0.f;
        g_Global.SpotLight.strength *= 0.f;
    }
    else if (m_Type == LIGHT_TYPE::SPOT)
    {
        g_Global.SpotLight = m_Light;

        g_Global.DirLight.strength *= 0.f;
        g_Global.PointLight.strength *= 0.f;
    }
}
