#include "pch.h"
#include "CElfilisA_SlashCombo.h"
#include "CElfilisFSM.h"
#include "CMomentaryObjScript.h"

CElfilisA_SlashCombo::CElfilisA_SlashCombo()
    : m_ComboLevel(0)
    , m_SpawnDist(0.f)
{
    m_StabRockPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisSlashStone.pref", L"prefab\\ElfilisSlashStone.pref");
}

CElfilisA_SlashCombo::~CElfilisA_SlashCombo()
{
}

void CElfilisA_SlashCombo::tick()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        Ready();
    }
    break;
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
    }
}

void CElfilisA_SlashCombo::Enter()
{
    m_ComboLevel = 0;
    m_Step = StateStep::Ready;
    Enter_Step();
}

void CElfilisA_SlashCombo::Exit()
{
    Exit_Step();
    ELFFSM->OffWeaponTrigger();
}

void CElfilisA_SlashCombo::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        if (m_ComboLevel == 0)
        {
            GetOwner()->Animator()->Play(ANIMPREFIX("SlashReady"), false);
        }
        else if (m_ComboLevel == 1)
        {
            GetOwner()->Animator()->Play(ANIMPREFIX("SlashChainReady"), false);
        }
        else if (m_ComboLevel == 2)
        {
            GetOwner()->Animator()->Play(ANIMPREFIX("SlashChainStabReady"), false);
        }

        ELFFSM->OnWeaponTrigger();
    }
    break;
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SlashStart"), false);
        //@Effect Halberd ∆Æ∑π¿œ ¿Ã∆Â∆Æ
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Slash"));
        m_SpawnDist = 0.f;
        m_PrevPos = GetOwner()->Transform()->GetWorldPos();
        m_PrevPos.y = 0.f;

        // Slash
        Vec3 Dir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
        Dir.y = 0.f;
        Dir.Normalize();
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->AddForce(Dir * 150.f, ForceMode::VelocityChange);

        //@Effect ∞¯±‚∆Œ ¿Ã∆Â∆Æ
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("SlashEnd"), false);

        // Up
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->AddForce(Vec3(0.f, 1000.f, 1.f), ForceMode::Impulse);
    }
    break;
    }
}

void CElfilisA_SlashCombo::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        if (m_ComboLevel == 2)
        {
            ELFFSM->AddComboLevel();
        }
    }
    break;
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::Wait:
        break;
    }
}

void CElfilisA_SlashCombo::Ready()
{
    RotateToPlayer();

    if (GetOwner()->Animator()->IsFinish())
    {
        if (m_ComboLevel < 2)
        {
            ChangeStep(StateStep::Start);
        }
        else if (m_ComboLevel == 2)
        {
            ELFFSM->ChangeStateGroup(ElfilisStateGroup::AirToGround, L"AIR_TOGROUND_STAB");
        }
    }
}

void CElfilisA_SlashCombo::Start()
{
    RotateToPlayer();
    GetOwner()->Rigidbody()->AddForce(Vec3(0.f, -1500.f, 1.f), ForceMode::Force);

    if (GetOwner()->Transform()->GetWorldPos().y <= 100.f)
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisA_SlashCombo::Progress()
{
    Vec3 CurPos = GetOwner()->Transform()->GetWorldPos() - ELFFSM->GetMapFloorOffset();
    CurPos.y = 0.f;

    // Slash Rock º“»Ø
    if (CurPos.Length() <= ELFFSM->GetMapSizeRadius())
    {
        float SpawnBetween = 30.f;
        m_SpawnDist += (CurPos - m_PrevPos).Length();

        if (m_SpawnDist >= SpawnBetween && m_StabRockPref != nullptr)
        {
            CGameObject* pRock = m_StabRockPref->Instantiate();
            CMomentaryObjScript* pScript = pRock->GetScript<CMomentaryObjScript>();
            m_SpawnDist -= SpawnBetween;

            if (pScript)
            {
                Vec3 NewPos = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
                NewPos.y = 0.f;
                NewPos += NewPos.Normalize() * -m_SpawnDist;
                NewPos += GetOwner()->Transform()->GetWorldPos();
                NewPos.y = 0.f;

                Vec3 NewDir = Vec3(GetRandomfloat(-100.f, 100.f), 0.f, GetRandomfloat(-100.f, 100.f)).Normalize();
                if (NewDir.Length() <= 0.f)
                {
                    NewDir = Vec3(1.f, 0.f, 1.f);
                }

                pRock->Transform()->SetWorldPos(NewPos);
                pRock->Transform()->Slerp(NewDir, 1.f);

                pScript->SetPlayTime(5.f);

                GamePlayStatic::SpawnGameObject(pRock, LAYER_DYNAMIC);
            }
        }
    }

    m_PrevPos = CurPos;

    // Change Step
    if (CurPos.Length() > ELFFSM->GetAirPos().z)
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisA_SlashCombo::Wait()
{
    if (GetOwner()->Transform()->GetWorldPos().y >= ELFFSM->GetAirPos().y)
    {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        m_ComboLevel++;

        ChangeStep(StateStep::Ready);
    }
}