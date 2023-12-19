#include "pch.h"
#include "CCamera.h"

#include "CTransform.h"

CCamera::CCamera()
    : CComponent(COMPONENT_TYPE::CAMERA)
    , m_FOV(0.f)
    , m_Width(0.f)
    , m_Scale(0.f)
    , m_AspectRatio(1.f)
    , m_Far(10000.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::finaltick()
{
    Vec3 vCamPos = Transform()->GetRelativePos();

    m_matView = XMMatrixIdentity();

    m_matView._41 = -vCamPos.x;
    m_matView._42 = -vCamPos.y;
    m_matView._43 = -vCamPos.z;

    m_matProj = XMMatrixIdentity();

    g_Transform.matView = m_matView;
    g_Transform.matProj = m_matProj;
}
