#include "pch.h"
#include "CElfilisG_SwordWaveLR.h"
#include "CElfilisFSM.h"
#include "CElfilisSwordSlashScript.h"
#include "CCameraController.h"

CElfilisG_SwordWaveLR::CElfilisG_SwordWaveLR()
    : m_bComboSuccess(false)
    , m_bFrmEnter(true)
{
    m_SwordSlash = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisSwordSlash.pref", L"prefab\\ElfilisSwordSlash.pref");
}

CElfilisG_SwordWaveLR::~CElfilisG_SwordWaveLR()
{
}

void CElfilisG_SwordWaveLR::tick()
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

void CElfilisG_SwordWaveLR::Enter()
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

void CElfilisG_SwordWaveLR::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRightStart"), false, false, 2.5f, 0.5f);

        // sound
        wstring Start = L"sound\\wav\\CharaBossChimera2\\0002_SwingStart.wav";
        GamePlayStatic::Play2DSound(Start, 1, SOUND_ELFILIS * 0.7f);
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ChainLR"), false, false, 1.5f, 0.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRight"), false);
        ELFFSM->OnWeaponTrigger();
        m_bFrmEnter = true;

        // sound
        wstring Start = L"sound\\wav\\CharaBossChimeraSoulCore\\0000_Slash.wav";
        GamePlayStatic::Play2DSound(Start, 1, SOUND_ELFILIS);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SwingRightEnd"), false, false, 1.5f);
    }
    break;
    }
}

void CElfilisG_SwordWaveLR::Exit_Step()
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
        ELFFSM->OffWeaponTrigger();

        // Spawn DropStar
        Vec3 SpawnDir = -(GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) + GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT)).Normalize();
        Vec3 SpawnPos = GetOwner()->Transform()->GetWorldPos() + SpawnDir * 100.f;
        ELFFSM->SpawnDropStar(SpawnPos);
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisG_SwordWaveLR::Start()
{
    // rotate
    RotateToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisG_SwordWaveLR::Wait()
{
    // rotate
    RotateToPlayer(DT * 3.f);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisG_SwordWaveLR::Progress()
{
    // rotate
    RotateToPlayer(DT * 3.f);

    // Spawn SwordSlash
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 11))
    {
        if (m_SwordSlash != nullptr)
        {
            CGameObject* pSlash = m_SwordSlash->Instantiate();
            CElfilisSwordSlashScript* pScript = pSlash->GetScript<CElfilisSwordSlashScript>();

            Vec3 InitPos = GetOwner()->Transform()->GetWorldPos();
            InitPos += GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT) * 30.f;

            pSlash->Transform()->SetWorldPos(InitPos);
            pScript->SetInitDir(GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT));

            GamePlayStatic::SpawnGameObject(pSlash, LAYER_MONSTERATK);
        }

        CAMERACTRL->Shake(0.3f, 30.f, 10.f);

        m_bFrmEnter = false;
    }

    // resize Hitbox
    CBoxCollider* pHitbox = ELFFSM->GetHitbox();
    if (pHitbox && !GetOwner()->Animator()->IsChainging())
    {
        if (GetOwner()->Animator()->GetClipFrameIndex() < 13)
        {
            pHitbox->GetOwner()->SetActive(true);
            pHitbox->Transform()->SetLocalPos(Vec3(0.71f, 1.f, 2.64f));
            pHitbox->Transform()->SetLocalRotation(Vec3(0.f));
            pHitbox->Transform()->SetLocalScale(Vec3(7.56f, 1.f, 4.81f));
        }
        else
        {
            pHitbox->GetOwner()->SetActive(false);
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        // Check Combo
        float Rand = 0.f;
        Rand = GetRandomfloat(1.f, 100.f);

        if (Rand <= 70.f)
        {
            m_bComboSuccess = true;
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_SWORDWAVE_FINISHLR");
        }
        else
        {
            ChangeStep(StateStep::End);
        }
    }
}

void CElfilisG_SwordWaveLR::End()
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