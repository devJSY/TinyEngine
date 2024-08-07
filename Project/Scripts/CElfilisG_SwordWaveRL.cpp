#include "pch.h"
#include "CElfilisG_SwordWaveRL.h"
#include "CElfilisFSM.h"
#include "CElfilisSwordSlashScript.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

CElfilisG_SwordWaveRL::CElfilisG_SwordWaveRL()
    : m_bComboSuccess(false)
    , m_bFrmEnter(true)
{
    m_SwordSlash = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisSwordSlash.pref", L"prefab\\ElfilisSwordSlash.pref");
}

CElfilisG_SwordWaveRL::~CElfilisG_SwordWaveRL()
{
}

void CElfilisG_SwordWaveRL::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Wait: {
        Wait();
    }
    break;
    case StateStep::Progress: {
        Progress();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CElfilisG_SwordWaveRL::Enter()
{
    if (ELFFSM->GetComboLevel() == 0)
    {
        m_Step = StateStep::Start;
    }
    else
    {
        m_Step = StateStep::Wait;
    }

    Enter_Step();
}

void CElfilisG_SwordWaveRL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingLeftStart"), false, false, 2.5f, 0.5f);
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ChainRL"), false, false, 1.5f, 0.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingLeft"), false);
        m_bFrmEnter = true;
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingLeftEnd"), false, false, 1.5f);
    }
    break;
    }
}

void CElfilisG_SwordWaveRL::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Wait:
        break;
    case StateStep::Progress: {
        if (m_bComboSuccess)
        {
            ELFFSM->AddComboLevel();
        }
        else
        {
            ELFFSM->ClearComboLevel();
        }
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_SwordWaveRL::Start()
{
    // rotate
    RotateToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisG_SwordWaveRL::Wait()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_SwordWaveRL::Progress()
{
    // Spawn SwordSlash

    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 10))
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
        // Check Combo
        float Rand = 0.f;
        Rand = GetRandomfloat(1.f, 100.f);

        if (Rand <= 70.f)
        {
            m_bComboSuccess = true;
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_SWORDWAVE_LR");
        }
        else
        {
            ChangeStep(StateStep::End);
        }
    }
}

void CElfilisG_SwordWaveRL::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ELFFSM->GetCurStateGroup())
        {
            ELFFSM->RepeatState();
        }
        else
        {
            ELFFSM->ChangeStateGroup(NextState);
        }
    }
}