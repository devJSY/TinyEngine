#include "pch.h"
#include "CMeshRender.h"

#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CRenderMgr.h"
#include "CEditorMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"
#include "CMaterial.h"

#include "CGameObject.h"
#include "CAnimator2D.h"
#include "CAnimator3D.h"
#include "CTransform.h"

#include "CConstBuffer.h"
#include "CDevice.h"

#include "CLevelMgr.h"

CMeshRender::CMeshRender()
    : CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}

void CMeshRender::finaltick()
{
    CRenderComponent::finaltick();

    // GamePlayStatic::DrawDebugCross(Transform()->GetWorldPos(), 1.f, Vec3(0.f, 1.f, 0.f), false);
}

void CMeshRender::render()
{
    if (nullptr == GetMesh())
        return;

    // Animatio2D 업데이트
    if (Animator2D())
    {
        Animator2D()->UpdateData();
    }

    // Animator3D 업데이트
    if (Animator3D())
    {
        Animator3D()->UpdateData();

        for (UINT i = 0; i < GetMtrlCount(); ++i)
        {
            if (nullptr == GetMaterial(i))
                continue;

            GetMaterial(i)->SetAnim3D(true); // Animation Mesh 알리기
            GetMaterial(i)->SetBoneCount(Animator3D()->GetBoneCount());
        }
    }

    Transform()->UpdateData();

    for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
    {
        if (nullptr == GetMaterial(i))
            continue;

        GetMaterial(i)->UpdateData();
        GetMesh()->render(i);
    }

    // Animation 관련 정보 제거
    if (Animator2D())
    {
        Animator2D()->Clear();
    }

    if (Animator3D())
    {
        Animator3D()->ClearData();
    }
}

void CMeshRender::render(Ptr<CMaterial> _mtrl)
{
    if (nullptr == GetMesh() || nullptr == _mtrl)
        return;

    // Animatio2D 업데이트
    if (Animator2D())
    {
        Animator2D()->UpdateData();
    }

    // Animator3D 업데이트
    if (Animator3D())
    {
        Animator3D()->UpdateData();

        _mtrl->SetAnim3D(true);
        _mtrl->SetBoneCount(Animator3D()->GetBoneCount());
    }

    Transform()->UpdateData();
    _mtrl->UpdateData();

    for (UINT i = 0; i < GetMesh()->GetSubsetCount(); ++i)
    {
        GetMesh()->render(i);
    }

    // Animation 관련 정보 제거
    if (Animator2D())
    {
        Animator2D()->Clear();
    }

    if (Animator3D())
    {
        Animator3D()->ClearData();
    }
}
