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
}

void CTransform::UpdateData()
{
    tTransform transform = {};
    transform.vWorldPos = m_vRelativePos;
    transform.vWorldScale = m_vRelativeScale;

    // 위치정보를 Transform 상수버퍼에 보내고, B0 레지스터에 바인딩 해둠
    CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
    pCB->SetData(&transform);
    pCB->UpdateData(0);
}