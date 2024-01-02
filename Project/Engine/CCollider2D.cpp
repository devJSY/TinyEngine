#include "pch.h"
#include "CCollider2D.h"

#include "CTransform.h"

CCollider2D::CCollider2D()
    : CComponent(COMPONENT_TYPE::COLLIDER2D)
    , m_CollisionCount(0)
    , m_bAbsolute(false)
    , m_Type(COLLIDER2D_TYPE::RECT)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::finaltick()
{
    // 충돌체의 로컬 행렬 계산
    m_matColWorld = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
    m_matColWorld *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

    // 충돌체에 적용시킬 GameObject 의 월드행렬
    const Matrix& matObjWorld = Transform()->GetWorldMat();

    // GameObject 의 Scale 에 영향을 받지 않으려는 경우
    if (m_bAbsolute)
    {
        Vec3 vWorldScale = Transform()->GetWorldScale();
        Matrix matScaleInv = XMMatrixScaling(vWorldScale.x, vWorldScale.y, vWorldScale.z);
        matScaleInv = XMMatrixInverse(nullptr, matScaleInv);

        // Object 의 Scale 의 역행렬을 사이에 배치해서 무효화 시킴
        m_matColWorld = m_matColWorld * matScaleInv * matObjWorld;
    }
    else
    {
        m_matColWorld *= matObjWorld;
    }

    // 충돌중이면 Red, 충돌하고 있지 않으면 Green
    if (0 == m_CollisionCount)
    {
        GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(0.f, 1.f, 0.f), false);
    }
    else
    {
        GamePlayStatic::DrawDebugRect(m_matColWorld, Vec3(1.f, 0.f, 0.f), false);
    }
}
