#include "pch.h"
#include "CMorphoAtkG_NormalNear_Atk2.h"
#include "CMorphoFSM.h"

CMorphoAtkG_NormalNear_Atk2::CMorphoAtkG_NormalNear_Atk2()
    : m_PrevDrag(0.f)
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

        // move
        Vec3 Dir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
        Dir.y = 0.f;
        Dir.Normalize();

        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        GetOwner()->Rigidbody()->AddForce(Dir * 15.f, ForceMode::Impulse);
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
        GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
        MRPFSM->OffWeaponRTrigger();

        if (MRPFSM->GetComboLevel() == 1)
        {
            MRPFSM->ClearComboLevel();
        }
        else
        {
            GetOwner()->Rigidbody()->SetVelocity(Vec3());
        }
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

    // Add drag
    float t = GetOwner()->Animator()->GetClipPlayRatio();
    float Ratio = t * XM_PI / 2.f;
    float NewDrag = 5.f * sinf(Ratio);
    GetOwner()->Rigidbody()->SetDrag(NewDrag);

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
