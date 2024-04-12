#include "pch.h"
#include "CEntetElevatorScript.h"

#include <Engine\\CAssetMgr.h>
#include <Engine\\components.h>
#include <Engine\\CAnim.h>
#include <Engine\\CPrefab.h>

#include "CPlayerScript.h"

CEntetElevatorScript::CEntetElevatorScript()
    : CScript(ENTETELEVATORSCRIPT)
    , m_IsPlayerSpawn(false)
{
}

CEntetElevatorScript::~CEntetElevatorScript()
{
}

void CEntetElevatorScript::begin()
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
        Animator2D()->LoadAnimation(L"AnimData\\BGObject\\GPE_Elevator.anim");
    }

    Animator2D()->Play(L"GPE_Elevator", false);

    // Transition Animation
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    int PostProcessIdx = pCurLevel->FindLayerIndexByName(L"PostProcess");
    if (-1 == PostProcessIdx)
        PostProcessIdx = LAYER_MAX - 1;

    CGameObject* TransitionFilterObj =
        CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\TransitionFilter.pref", L"prefab\\TransitionFilter.pref")->Instantiate();
    TransitionFilterObj->Animator2D()->Play(L"Transition01_Reverse", false);
    GamePlayStatic::SpawnGameObject(TransitionFilterObj, PostProcessIdx);
}

void CEntetElevatorScript::tick()
{
    if (45 == Animator2D()->GetCurAnim()->GetCurFrmIdx() && !m_IsPlayerSpawn)
    {
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        int PlayerLayerIdx = pCurLevel->FindLayerIndexByName(L"Player");
        if (-1 == PlayerLayerIdx)
            PlayerLayerIdx = 0;

        CGameObject* pPlayer = CGameManagerScript::GetInset()->GetPlayer();
        pPlayer->Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(0.f, -158.f, 0.f));
        CPlayerScript* PlayerScript = pPlayer->GetScript<CPlayerScript>();
        PlayerScript->ChangeState(PLAYER_STATE::Elevator_Out);

        GamePlayStatic::SpawnGameObject(pPlayer, PlayerLayerIdx);
        m_IsPlayerSpawn = true;
    }
}
