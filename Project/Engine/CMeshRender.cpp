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

void CMeshRender::UpdateData()
{
    if (nullptr != GetMaterial())
    {
        GetMaterial()->UpdateData();
    }

    Transform()->UpdateData();
}

void CMeshRender::finaltick()
{
    CRenderComponent::finaltick();

    GamePlayStatic::DrawDebugCross(Transform()->GetWorldPos(), 1.f, Vec3(0.f, 1.f, 0.f), false);
}

void CMeshRender::render()
{
    if (nullptr == GetMesh() || nullptr == GetMaterial())
        return;

    // Animatio2D 보유한 경우
    if (Animator2D())
        Animator2D()->UpdateData();

    UpdateData();

    GetMesh()->render();

    // Animation 관련 정보 제거
    if (Animator2D())
        Animator2D()->Clear();
}

void CMeshRender::render(Ptr<CMaterial> _mtrl)
{
    if (nullptr == GetMesh() || nullptr == _mtrl)
        return;

    // Animatio2D 보유한 경우
    if (Animator2D())
    {
        Animator2D()->UpdateData();
    }

    _mtrl->UpdateData();
    Transform()->UpdateData();

    GetMesh()->render();

    // Animation 관련 정보 제거
    if (Animator2D())
    {
        Animator2D()->Clear();
    }
}
