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

    ELFFSM->ClearComboLevel();
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
        ELFFSM->OnWeaponTrigger();
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
    case StateStep::Progress: {
        ELFFSM->OffWeaponTrigger();
    }
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

    // resize Hitbox
    CBoxCollider* pHitbox = ELFFSM->GetHitbox();
    if (pHitbox && !GetOwner()->Animator()->IsChainging())
    {
        if (GetOwner()->Animator()->GetClipFrameIndex() > 32 && GetOwner()->Animator()->GetClipFrameIndex() < 42)
        {
            pHitbox->GetOwner()->SetActive(true);
            pHitbox->Transform()->SetLocalPos(Vec3(0.f, 1.f, 0.f));
            pHitbox->Transform()->SetLocalRotation(Vec3(0.f));
            pHitbox->Transform()->SetLocalScale(Vec3(7.5f, 1.f, 7.5f));
        }
        else
        {
            pHitbox->GetOwner()->SetActive(false);
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_SWORDWAVE_STORM");
    }
}