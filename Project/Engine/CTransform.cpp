#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
    : CComponent(COMPONENT_TYPE::TRANSFORM)
    , m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
{
}

CTransform::~CTransform()
{
}

void CTransform::finaltick()
{
    m_matWorld = XMMatrixIdentity();

    Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

    Matrix matRotX = XMMatrixRotationX(m_vRelativeRotation.x);
    Matrix matRotY = XMMatrixRotationY(m_vRelativeRotation.y);
    Matrix matRotZ = XMMatrixRotationZ(m_vRelativeRotation.z);

    Matrix matTranslation = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

    m_matWorld = matScale * matRotX * matRotY * matRotZ * matTranslation;

    // 물체의 방향값을 다시 계산한다.
    m_arrDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
    m_arrDir[(UINT)DIR_TYPE::UP] = Vec3(0.f, 1.f, 0.f);
    m_arrDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

    // Vec3 를 Vec4 타입으로 확장해서 행렬을 적용시켜야 함
    // XMVector3TransformCoord	- w 를 1로 확장
    // XMVector3TransformNormal - w 를 0으로 확장
    // mul(float4(_in.vPos, 1 or 0), g_matWorld);
    // 적용 받을 상태행렬의 이동을 적용할지 말지 결정
    for (int i = 0; i < 3; ++i)
    {
        // m_matWorld 행렬에 크기정보가 있을 수 있기 때문에 다시 길이를 1로 정규화 시킨다.
        m_arrDir[i] = XMVector3TransformNormal(m_arrDir[i], m_matWorld);
        m_arrDir[i].Normalize();
    }
}

void CTransform::UpdateData()
{
    g_Transform.matWorld = m_matWorld;
    g_Transform.matWV = g_Transform.matWorld * g_Transform.matView;
    g_Transform.matWVP = g_Transform.matWV * g_Transform.matProj;


    // 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩 해둠
    CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
    pCB->SetData(&g_Transform);
    pCB->UpdateData(0);
}