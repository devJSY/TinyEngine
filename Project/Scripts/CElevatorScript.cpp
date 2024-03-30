#include "pch.h"
#include "CElevatorScript.h"

#include <Engine\\CAssetMgr.h>
#include <Engine\\components.h>
#include <Engine\\CAnim.h>
#include <Engine\\CPrefab.h>

#include "CPlayerScript.h"

CElevatorScript::CElevatorScript()
    : CScript(ELEVATORSCRIPT)
    , m_IsPlayerSpawn(false)
{
}

CElevatorScript::~CElevatorScript()
{
}

void CElevatorScript::begin()
{
    if (MeshRender())
        MeshRender()->CreateDynamicMaterial();

    if (Animator2D())
    {
        //Animator2D()->LoadAnimation(L"AnimData\\BGObject\\GPE_Elevator.anim");
        Animator2D()->Play(L"GPE_Elevator", false);
    }
}

void CElevatorScript::tick()
{
    if (Animator2D())
    {
        if (45 == Animator2D()->GetCurAnim()->GetCurFrmIdx() && !m_IsPlayerSpawn)
        {
            int PlayerLayerIdx = 0;
            CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

            for (int i = 0; i < LAYER_MAX; i++)
            {
                if (L"Player" == pCurLevel->GetLayer(i)->GetName())
                {
                    PlayerLayerIdx = i;
                    break;
                }
            }

            CGameObject* pPlayer = CGameManagerScript::GetInset()->GetPlayer();
            pPlayer->Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(0.f, -158.f, 0.f));
            CPlayerScript* PlayerScript = pPlayer->GetScript<CPlayerScript>();
            PlayerScript->ChangeState(PLAYER_STATE::Elevator_Out);

            GamePlayStatic::SpawnGameObject(pPlayer, PlayerLayerIdx);
            m_IsPlayerSpawn = true;
        }
    }
}
