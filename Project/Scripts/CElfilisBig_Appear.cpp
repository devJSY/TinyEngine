#include "pch.h"
#include "CElfilisBig_Appear.h"
#include "CBossMgr.h"
#include "CElfilisFSM.h"
#include "CElfilisBigFSM.h"

CElfilisBig_Appear::CElfilisBig_Appear()
    : m_AccTime(0.f)
    , m_PrevAlpha(1.f)
{
}

CElfilisBig_Appear::~CElfilisBig_Appear()
{
}

void CElfilisBig_Appear::tick()
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

    // Change State
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeState(L"IDLE");
    }
}

void CElfilisBig_Appear::Enter()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("BigAppear"), false, false, 4.f, 0.f);

    ELFFSM->GetBigBossFSM()->OffWeaponCollider();

    m_AccTime = 0.f;
    m_PrevAlpha = *(float*)(GetOwner()->MeshRender()->GetMaterial(0)->GetScalarParam(SCALAR_PARAM::FLOAT_2));
}

void CElfilisBig_Appear::Exit()
{
    ELFFSM->GetBigBossFSM()->OnWeaponCollider();
}