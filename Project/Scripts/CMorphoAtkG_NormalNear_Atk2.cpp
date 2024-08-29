#include "pch.h"
#include "CMorphoAtkG_NormalNear_Atk2.h"
#include "CMorphoFSM.h"
#include "CCameraController.h"

CMorphoAtkG_NormalNear_Atk2::CMorphoAtkG_NormalNear_Atk2()
{
}

CMorphoAtkG_NormalNear_Atk2::~CMorphoAtkG_NormalNear_Atk2()
{
}

void CMorphoAtkG_NormalNear_Atk2::tick()
{
    switch (m_Step)
    {
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

void CMorphoAtkG_NormalNear_Atk2::Enter()
{
    m_Step = StateStep::Progress;
    Enter_Step();
}

void CMorphoAtkG_NormalNear_Atk2::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack2"), false, false, 1.5f);
        MRPFSM->OnWeaponRTrigger();

        // move dir
        m_ForceDir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
        m_ForceDir.y = 0.f;
        m_ForceDir.Normalize();

        CAMERACTRL->Shake(0.3f, 20.f, 20.f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack2End"), false, false, 1.5f);
    }
    break;
    }
}

void CMorphoAtkG_NormalNear_Atk2::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Progress: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
        MRPFSM->OffWeaponRTrigger();

        if (MRPFSM->GetComboLevel() == 1)
        {
            MRPFSM->ClearComboLevel();
        }
        else
        {
            GetOwner()->Rigidbody()->SetVelocity(Vec3());
        }

        // Spawn DropStar
        Vec3 SpawnPos = GetOwner()->Transform()->GetWorldPos();
        SpawnPos += GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 50.f;
        MRPFSM->SpawnDropStar(SpawnPos);
    }
    break;
    case StateStep::End: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
    }
    break;
    }
}

void CMorphoAtkG_NormalNear_Atk2::Progress()
{
    RotateToPlayer();

    // move
    float t = GetOwner()->Animator()->GetClipPlayRatio();
    float NewSpeed = 15.f * sinf(t * XM_PI / 2.f);
    Vec3 NewVeloc = m_ForceDir * NewSpeed;
    GetOwner()->Rigidbody()->SetVelocity(NewVeloc);

    // Change Step
    if (GetOwner()->Animator()->IsFinish())
    {
        if (MRPFSM->GetComboLevel() == 1)
        {
            ChangeStep(StateStep::End);
        }
        else
        {
            MRPFSM->ChangeState(L"ATKG_NORMALNEAR_ATK3");
        }
    }
}

void CMorphoAtkG_NormalNear_Atk2::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}
