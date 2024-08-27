#include "pch.h"
#include "CElfilisG_SwordWaveStorm.h"
#include "CElfilisFSM.h"
#include "CElfilisStormScript.h"
#include "CCameraController.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

CElfilisG_SwordWaveStorm::CElfilisG_SwordWaveStorm()
    : m_AccTime(0.f)
    , m_PlayTime(2.f)
    , m_bFrmEnter(true)
    , m_Storm(nullptr)
{
    m_StormPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisStorm.pref");
}

CElfilisG_SwordWaveStorm::~CElfilisG_SwordWaveStorm()
{
}

void CElfilisG_SwordWaveStorm::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Progress: {
        Progress();
    }
    break;
    case StateStep::Wait: {
        Wait();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CElfilisG_SwordWaveStorm::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpikeStart"), false);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpike"), false);
        ELFFSM->OnWeaponTrigger();
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpikeWait"));
        m_AccTime = 0.f;
        m_bFrmEnter = true;

        if (m_Storm)
        {
            m_PlayTime = m_Storm->GetRemainTime();
        }
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionSpikeEnd"), false);
    }
    break;
    }
}

void CElfilisG_SwordWaveStorm::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress: {
        ELFFSM->OffWeaponTrigger();
    }
    break;
    case StateStep::Wait:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_SwordWaveStorm::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_SwordWaveStorm::Progress()
{
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 20))
    {
        if (m_StormPref != nullptr)
        {
            // spawn strom
            RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(GetOwner()->Transform()->GetWorldPos(), Vec3(0.f, -1.f, 0.f), 100.f,
                                                             vector<wstring>{L"World Static"});

            for (int i = 0; i < 7; ++i)
            {
                CGameObject* pStorm = m_StormPref->Instantiate();
                Vec3 m_InitPos =
                    (Hit.pCollisionObj) ? Hit.Point : Vec3(GetOwner()->Transform()->GetWorldPos().x, 0.f, GetOwner()->Transform()->GetWorldPos().z);

                m_Storm = pStorm->GetScript<CElfilisStormScript>();
                if (m_Storm)
                {
                    m_Storm->SetInitPosition(m_InitPos);
                    m_Storm->SetStormIdx(i);
                }

                GamePlayStatic::SpawnGameObject(pStorm, LAYER_MONSTERATK_TRIGGER);
            }
        }

        // Camera Shake
        CAMERACTRL->Shake(0.4f, 60.f, 40.f);

        m_bFrmEnter = false;
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisG_SwordWaveStorm::Wait()
{
    m_AccTime += DT;

    if (m_AccTime > m_PlayTime)
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisG_SwordWaveStorm::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ELFFSM->GetCurStateGroup())
        {
            ELFFSM->RepeatState(L"GROUND_ATK_SWORDWAVE_RL");
        }
        else
        {
            ELFFSM->ChangeStateGroup(NextState);
        }
    }
}