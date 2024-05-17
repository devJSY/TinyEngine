#include "pch.h"
#include "CFrustum.h"

#include "CRenderMgr.h"
#include "CLevelMgr.h"

#include "CCamera.h"
#include "CLevel.h"

CFrustum::CFrustum(CCamera* _pOwnerCam)
    : m_pOwnerCam(_pOwnerCam)
    , m_arrFace{}
    , m_arrProj{}
{
    // 투영공간 좌표
    //     4 ------ 5
    //     |        |  Far
    //   / |        |
    //	/  7 ------ 6
    // /      /
    // 0 -- 1     /
    // |    |    / Near
    // 3 -- 2
    m_arrProj[0] = Vec3(-1.f, 1.f, 0.f);
    m_arrProj[1] = Vec3(1.f, 1.f, 0.f);
    m_arrProj[2] = Vec3(1.f, -1.f, 0.f);
    m_arrProj[3] = Vec3(-1.f, -1.f, 0.f);

    m_arrProj[4] = Vec3(-1.f, 1.f, 1.f);
    m_arrProj[5] = Vec3(1.f, 1.f, 1.f);
    m_arrProj[6] = Vec3(1.f, -1.f, 1.f);
    m_arrProj[7] = Vec3(-1.f, -1.f, 1.f);
}

CFrustum::~CFrustum()
{
}

void CFrustum::finaltick()
{
    Vec3 vWorldPos[8] = {};

    Matrix matPVInv = m_pOwnerCam->GetProjInvMat() * m_pOwnerCam->GetViewInvMat();

    for (int i = 0; i < 8; ++i)
    {
        vWorldPos[i] = XMVector3TransformCoord(m_arrProj[i], matPVInv);
    }

    m_arrFace[FT_NEAR] = XMPlaneFromPoints(vWorldPos[0], vWorldPos[1], vWorldPos[2]);
    m_arrFace[FT_FAR] = XMPlaneFromPoints(vWorldPos[7], vWorldPos[6], vWorldPos[5]);
    m_arrFace[FT_LEFT] = XMPlaneFromPoints(vWorldPos[4], vWorldPos[0], vWorldPos[7]);
    m_arrFace[FT_RIGHT] = XMPlaneFromPoints(vWorldPos[1], vWorldPos[5], vWorldPos[6]);
    m_arrFace[FT_TOP] = XMPlaneFromPoints(vWorldPos[4], vWorldPos[5], vWorldPos[1]);
    m_arrFace[FT_BOT] = XMPlaneFromPoints(vWorldPos[2], vWorldPos[6], vWorldPos[7]);

    if (0 > m_pOwnerCam->GetOwner()->Camera()->GetCameraPriority() || CRenderMgr::GetInst()->GetEditorCamera() == m_pOwnerCam ||
        CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY)
    {
        return;
    }

    GamePlayStatic::DrawDebugFrustum(vWorldPos, Vec3(1.f, 1.f, 1.f), true);
}

bool CFrustum::FrustumCheckByPoint(Vec3 _vWorldPos)
{
    for (int i = 0; i < FT_END; ++i)
    {
        // 임의의 점과 해당 평면의 법선벡터와 내적
        Vec3 vNormal = m_arrFace[i];
        vNormal.Normalize();
        float fDot = vNormal.Dot(_vWorldPos);

        // 내적한 값과 원점에서 해당 평면까지의 최단거리(D) 와 비교
        if (fDot + m_arrFace[i].w > 0)
            return false;
    }

    return true;
}

bool CFrustum::FrustumCheckBySphere(Vec3 _vWorldPos, float _fRadius)
{
    for (int i = 0; i < FT_END; ++i)
    {
        // 임의의 점과 해당 평면의 법선벡터와 내적
        Vec3 vNormal = m_arrFace[i];
        vNormal.Normalize();
        float fDot = vNormal.Dot(_vWorldPos);

        // 내적한 값과 원점에서 해당 평면까지의 최단거리(D) 와 비교
        if (fDot + m_arrFace[i].w > _fRadius)
            return false;
    }

    return true;
}
