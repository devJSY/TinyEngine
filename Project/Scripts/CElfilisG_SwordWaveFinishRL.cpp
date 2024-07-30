#include "pch.h"
#include "CElfilisG_SwordWaveFinishRL.h"
#include "CElfilisFSM.h"
#include "CElfilisSwordSlashScript.h"

CElfilisG_SwordWaveFinishRL::CElfilisG_SwordWaveFinishRL()
    : m_bFrmEnter(true)
{
    m_SwordSlash = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisSwordSlash.pref", L"prefab\\ElfilisSwordSlash.pref");
}
CElfilisG_SwordWaveFinishRL::~CElfilisG_SwordWaveFinishRL()
{
}

void CElfilisG_SwordWaveFinishRL::tick()
{
    switch (m_Step)
    {
    case StateStep::Wait: {
        Wait();
    }
    break;
    case StateStep::Progress: {
        Progress();
    }
    break;
    }
}

void CElfilisG_SwordWaveFinishRL::Enter()
{
    m_Step = StateStep::Wait;
    Enter_Step();

    ELFFSM->SetComboLevel(0);
}

void CElfilisG_SwordWaveFinishRL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ChainRL"), false, false, 1.5f, 0.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingFinishLeft"), false);
        m_bFrmEnter = true;
    }
    break;
    }
}

void CElfilisG_SwordWaveFinishRL::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Wait:
        break;
    case StateStep::Progress:
        break;
    }
}

void CElfilisG_SwordWaveFinishRL::Wait()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_SwordWaveFinishRL::Progress()
{
    // Spawn SwordSlash
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 36))
    {
        if (m_SwordSlash != nullptr)
        {
            CGameObject* pSlash = m_SwordSlash->Instantiate();
            CElfilisSwordSlashScript* pScript = pSlash->GetScript<CElfilisSwordSlashScript>();

            if (pScript)
            {
                Vec3 InitPos = GetOwner()->Transform()->GetWorldPos();
                InitPos += GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 30.f;

                pSlash->Transform()->SetWorldPos(InitPos);
                pScript->SetInitDir(GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT));
            }

            GamePlayStatic::SpawnGameObject(pSlash, LAYER_MONSTERATK);
        }

        m_bFrmEnter = false;
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ELFFSM->ChangeStateGroup_SetState(ElfilisStateGroup::GroundAtk, L"GROUND_ATK_SWORDWAVE_STORM");
    }
}