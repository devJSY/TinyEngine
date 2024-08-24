#include "pch.h"
#include "CElfilisBig_Roar.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CElfilisBigFSM.h"

CElfilisBig_Roar::CElfilisBig_Roar()
    : m_PrevAlpha(0.f)
    , m_AccTime(0.f)
{
}

CElfilisBig_Roar::~CElfilisBig_Roar()
{
}

void CElfilisBig_Roar::tick()
{
    // Fade in
    float FadeTime = 2.f;
    m_AccTime += DT;

    if (m_AccTime <= FadeTime)
    {
        float t = m_AccTime / FadeTime;
        ELFFSM->GetBigBossFSM()->SetMtrlTransparent(m_PrevAlpha * t);
    }
    else
    {
        ELFFSM->GetBigBossFSM()->SetMtrlTransparent(m_PrevAlpha);
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        if (ELFFSM->IsPattern(ElfilisPatternType::Appear2, 1))
        {
            ELFBIGFSM->SetComboLevel(2);
            ChangeState(L"SWING_R");
        }
        else
        {
            ChangeState(L"IDLE");
        }
    }
}

void CElfilisBig_Roar::Enter()
{
    GetOwner()->SetActive(true);
    GetOwner()->Animator()->Play(ANIMPREFIX("BigRoar"), false, false, 2.5f, 0.f);

    //@ Effect 나타나는 Burning 효과
    m_AccTime = 0.f;
    m_PrevAlpha = *(float*)(GetOwner()->MeshRender()->GetMaterial(0)->GetScalarParam(SCALAR_PARAM::FLOAT_2));
}

void CElfilisBig_Roar::Exit()
{
}