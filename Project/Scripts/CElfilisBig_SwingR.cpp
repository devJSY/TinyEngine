#include "pch.h"
#include "CElfilisBig_SwingR.h"
#include "CElfilisFSM.h"
#include "CElfilisBigFSM.h"
#include "CMomentaryObjScript.h"

CElfilisBig_SwingR::CElfilisBig_SwingR()
    : m_bComboSuccess(false)
    , m_bFrmEnter(true)
    , m_ProgressChangeDuration(0.1f)
{
    m_Rock = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisMapRock.pref", L"prefab\\ElfilisMapRock.pref");
}

CElfilisBig_SwingR::~CElfilisBig_SwingR()
{
}

void CElfilisBig_SwingR::tick()
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
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CElfilisBig_SwingR::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        if (ELFBIGFSM->GetComboLevel() == 0)
        {
            GetOwner()->Animator()->Play(ANIMPREFIX("BigSwingRightStart"), false, false, 4.f, 2.f);
        }
        else
        {
            //GetOwner()->Animator()->Play(ANIMPREFIX("BigChainLR"), false, false, 4.f);
            m_ProgressChangeDuration = 1.f;
            ChangeStep(StateStep::Progress);
        }
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("BigSwingRight"), false, false, 4.f, m_ProgressChangeDuration);
        m_bFrmEnter = true;

        // Elfilis ÇÏ´Ã·Î ÀÌµ¿
        if (ELFFSM->IsPattern(ElfilisPatternType::BigCombo, 1)
            || ELFFSM->IsPattern(ElfilisPatternType::Appear2, 1))
        {
            ELFFSM->ProcPatternStep();
        }
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("BigSwingRightEnd"), false, false, 4.f);
    }
    break;
    }
}

void CElfilisBig_SwingR::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress: {
        if (m_bComboSuccess)
        {
            ELFBIGFSM->AddComboLevel();
        }
        else
        {
            ELFBIGFSM->ClearComboLevel();
        }
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisBig_SwingR::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisBig_SwingR::Progress()
{
    // Spawn Rock
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 240))
    {
        if (m_Rock != nullptr)
        {
            CGameObject* pRock = m_Rock->Instantiate();
            CMomentaryObjScript* pScript = pRock->GetScript<CMomentaryObjScript>();

            if (pScript)
            {
                // pScript->AddEffect(MomentaryEffectType::AppearScaling);
                pScript->SetPlayTime(3.5f);
            }

            GamePlayStatic::SpawnGameObject(pRock, LAYER_DYNAMIC);
        }

        m_bFrmEnter = false;
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        // Check Combo
        if (ELFBIGFSM->GetComboLevel() == 0)
        {
            m_bComboSuccess = true;
            ELFBIGFSM->ChangeState(L"SWING_L");
        }
        else if (ELFBIGFSM->GetComboLevel() == 2)
        {
            m_bComboSuccess = true;
            ELFBIGFSM->ChangeState(L"SWING_FINISHL");
        }
        else
        {
            ChangeStep(StateStep::End);
        }
    }
}

void CElfilisBig_SwingR::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}