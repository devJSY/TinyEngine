#include "pch.h"
#include "CLevelChangeScript.h"
#include <Engine\\CRenderMgr.h>
#include <Engine\\CAnim.h>
#include <Engine\\CLevelSaveLoad.h>
#include <Engine\\components.h>
#include <Engine\\CDevice.h>
#include <Engine\\CEditorMgr.h>

CLevelChangeScript::CLevelChangeScript()
    : CScript(LEVELCHANGESCRIPT)
    , m_ChangeLevelName()
{
}

CLevelChangeScript::~CLevelChangeScript()
{
}

void CLevelChangeScript::tick()
{
    if (m_ChangeLevelName.empty())
        return;

    if (Animator2D()->IsFinish())
    {
        GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(ToWstring(m_ChangeLevelName)), LEVEL_STATE::PLAY);
    }
}

void CLevelChangeScript::ChangeLevel(const std::string& LevelName)
{
    if (!m_ChangeLevelName.empty()) // 레벨 변경중인 상태
        return;

    m_ChangeLevelName = LevelName;

    if (nullptr == Animator2D())
        GetOwner()->AddComponent(new CAnimator2D);

    Animator2D()->LoadAnimation(L"HaveANiceDeath\\AnimData\\UI\\Title\\Transition.anim");
    Animator2D()->Play(L"Transition", false);

    if (nullptr == MeshRender())
        GetOwner()->AddComponent(new CMeshRender);

    MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));
    MeshRender()->CreateDynamicMaterial();

    Transform()->SetRelativePos(CRenderMgr::GetInst()->GetMainCamera()->GetOwner()->Transform()->GetRelativePos());
    Vec2 resolution = CDevice::GetInst()->GetRenderResolution();

    Transform()->SetRelativeScale(Vec3(resolution.x, resolution.y, 1.f));
}
