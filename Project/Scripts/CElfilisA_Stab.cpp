#include "pch.h"
#include "CElfilisA_Stab.h"
#include "CElfilisFSM.h"
#include "CElfilisUnit.h"
#include "CMomentaryObjScript.h"

#include "CCameraController.h"

CElfilisA_Stab::CElfilisA_Stab()
{
    m_StabRockPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisStabStone.pref", L"prefab\\ElfilisStabStone.pref");
}

CElfilisA_Stab::~CElfilisA_Stab()
{
}

void CElfilisA_Stab::tick()
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
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CElfilisA_Stab::Enter()
{
    if (ELFFSM->GetComboLevel() == 0)
    {
        m_Step = StateStep::Ready;
    }
    else
    {
        ELFFSM->ClearComboLevel();
        m_Step = StateStep::Start;
    }

    ELFFSM->OnWeaponTrigger();
    Enter_Step();
}

void CElfilisA_Stab::Exit()
{
    Exit_Step();
    ELFFSM->OffWeaponTrigger();

    if (m_Step < StateStep::Wait)
    {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
        GetOwner()->Rigidbody()->SetFreezeRotation(AXIS_TYPE::X, false);
        GetOwner()->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, false);
        GetOwner()->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Z, false);
    }
}

void CElfilisA_Stab::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        GetOwner()->Animator()->Play(ANIMPREFIX("StabReady"), false);
    }
    break;
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("StabStart"), false);
        GetOwner()->Rigidbody()->SetFreezeRotation(AXIS_TYPE::X, true);
        GetOwner()->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, true);
        GetOwner()->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Z, true);

        // Slash
        CGameObject* Halberd = GetOwner()->GetChildObject(L"Halberd");
        Vec3 HalberdTop = Halberd->Transform()->GetWorldPos();
        HalberdTop += Halberd->Transform()->GetWorldDir(DIR_TYPE::UP) *
                      (Halberd->BoxCollider()->GetCenter().y + Halberd->BoxCollider()->GetSize().y * Halberd->Transform()->GetWorldScale().y / 2.f);
        Vec3 Dir = (PLAYER->Transform()->GetWorldPos() - HalberdTop).Normalize();
        GetOwner()->Rigidbody()->AddForce(Dir * 1800.f, ForceMode::Impulse);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("StabWait"));
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Stab"), false);
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
        GetOwner()->Rigidbody()->SetFreezeRotation(AXIS_TYPE::X, false);
        GetOwner()->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Y, false);
        GetOwner()->Rigidbody()->SetFreezeRotation(AXIS_TYPE::Z, false);

        // Stab Rock ¼ÒÈ¯
        if (m_StabRockPref != nullptr)
        {
            CGameObject* pRock = m_StabRockPref->Instantiate();
            CMomentaryObjScript* pScript = pRock->GetScript<CMomentaryObjScript>();

            Vec3 NewPos = GetOwner()->Transform()->GetWorldPos();
            NewPos.y = 0.f;
            Vec3 NewDir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            NewDir.y = 0.f;
            NewDir.Normalize();

            pRock->Transform()->SetWorldPos(NewPos);
            pRock->Transform()->Slerp(NewDir, 1.f);

            pScript->SetPlayTime(5.f);

            GamePlayStatic::SpawnGameObject(pRock, LAYER_DYNAMIC);
        }

        // Åõ Å¸°Ù
        CAMERACTRL->SetElfilisTwoTarget();
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("StabEnd"), false);
    }
    break;
    }
}

void CElfilisA_Stab::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Ready:
        break;
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::Wait:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisA_Stab::Ready()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Start);
    }
}

void CElfilisA_Stab::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisA_Stab::Progress()
{
    Vec3 Veloc = GetOwner()->Rigidbody()->GetVelocity();
    if (Veloc.Length() <= 100.f)
    {
        Veloc = Veloc.Normalize() * 100.f;
        GetOwner()->Rigidbody()->SetVelocity(Veloc);
    }

    if (ELFFSM->IsGround())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisA_Stab::Wait()
{
    GetOwner()->Rigidbody()->SetVelocity(Vec3());

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisA_Stab::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup(NextState);
    }
}