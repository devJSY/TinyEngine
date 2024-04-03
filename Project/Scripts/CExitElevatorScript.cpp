#include "pch.h"
#include "CExitElevatorScript.h"
#include "CLevelChangeScript.h"
#include "CPlayerScript.h"

CExitElevatorScript::CExitElevatorScript()
    : CScript(EXITELEVATORSCRIPT)
    , m_bExit(false)
{
}

CExitElevatorScript::~CExitElevatorScript()
{
}

void CExitElevatorScript::begin()
{
    if (nullptr == MeshRender())
    {
        GetOwner()->AddComponent(new CMeshRender);
        MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
        MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DLightMtrl"));

        Transform()->SetRelativeScale(Vec3(1024.f, 1024.f, 1.f));
    }

    MeshRender()->CreateDynamicMaterial();

    if (nullptr == Animator2D())
    {
        GetOwner()->AddComponent(new CAnimator2D);
        Animator2D()->LoadAnimation(L"AnimData\\BGObject\\GPE_Elevator_Reverse.anim");
        Animator2D()->Play(L"GPE_Elevator_Reverse", false);
    }
}

void CExitElevatorScript::tick()
{
    if (!m_bExit)
    {
        Animator2D()->Play(L"GPE_Elevator_Reverse", false);
    }

    if (Animator2D()->IsFinish())
    {
        const wstring& CurLevelName = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();

        if (L"Hall of Eternity Floor 0.tLevel" == CurLevelName)
            CGameManagerScript::GetInset()->ChangeLevel("Hall of Eternity Floor 1");
        else if (L"Hall of Eternity Floor 1.tLevel" == CurLevelName)
            CGameManagerScript::GetInset()->ChangeLevel("Home Level");
    }
}

void CExitElevatorScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (L"Player" != _OtherCollider->GetOwner()->GetName())
        return;

    m_bExit = true;
    CGameObject* pPlayer = CGameManagerScript::GetInset()->GetPlayer();
    pPlayer->GetScript<CPlayerScript>()->ChangeState(PLAYER_STATE::Elevator_In);
}
