#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CTransform.h"

CCamera::CCamera()
    : CComponent(COMPONENT_TYPE::CAMERA)
    , m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
    , m_FOV(XM_PI / 2.f)
    , m_Width(0.f)
    , m_Scale(1.f)
    , m_AspectRatio(1.f)
    , m_Far(10000.f)
{
    Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
    m_AspectRatio = vResol.x / vResol.y;
}

CCamera::~CCamera()
{
}

void CCamera::finaltick()
{
    // 뷰 행렬을 계산한다.
    // 카메라를 원점으로 이동시키는 이동 행렬
    Vec3 vCamPos = Transform()->GetRelativePos();
    Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

    // 카메라의 각 우, 상, 전 방 방향을 기저축이랑 일치시키도록 회전하는 회전행렬
    Vec3 vRight = Transform()->GetDir(DIR_TYPE::RIGHT);
    Vec3 vUp = Transform()->GetDir(DIR_TYPE::UP);
    Vec3 vFront = Transform()->GetDir(DIR_TYPE::FRONT);

    Matrix matRotate = XMMatrixIdentity();
    matRotate._11 = vRight.x;
    matRotate._12 = vUp.x;
    matRotate._13 = vFront.x;

    matRotate._21 = vRight.y;
    matRotate._22 = vUp.y;
    matRotate._23 = vFront.y;

    matRotate._31 = vRight.z;
    matRotate._32 = vUp.z;
    matRotate._33 = vFront.z;

    // 이동 x 회전 = view 행렬
    m_matView = matTrans * matRotate;

    // 투영 방식에 따른 투영 행렬을 계산한다.
    m_matProj = XMMatrixIdentity();

    if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
    {
        // 직교투영
        Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
        m_matProj = XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far);
    }
    else
    {
        // 원근투영
        m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
    }

    // 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
    g_Transform.matView = m_matView;
    g_Transform.matProj = m_matProj;
}
