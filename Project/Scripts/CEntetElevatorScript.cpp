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
    GamePlayStatic::Play2DSound(L"sound\\BGObject\\GPE_Elevator\\GPE_Elevator_Open_Up_01.wav", 1, 0.5f);
}

void CEntetElevatorScript::tick()
{
    if (!m_IsPlayerSpawn && 45 == Animator2D()->GetCurAnim()->GetCurFrmIdx())
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
        GamePlayStatic::Play2DSound(L"sound\\BGObject\\GPE_Elevator\\GPE_Elevator_Open_Door_01.wav", 1, 0.5f);
        m_IsPlayerSpawn = true;
    }
}
