#include "pch.h"
#include "CMorphoAtkG_NormalNear_Atk3.h"
#include "CMorphoFSM.h"

CMorphoAtkG_NormalNear_Atk3::CMorphoAtkG_NormalNear_Atk3()
    : m_PrevDrag(0.f)
{
}

CMorphoAtkG_NormalNear_Atk3::~CMorphoAtkG_NormalNear_Atk3()
{
}

void CMorphoAtkG_NormalNear_Atk3::tick()
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

void CMorphoAtkG_NormalNear_Atk3::Enter()
{
    m_Step = StateStep::Progress;
    Enter_Step();
}

void CMorphoAtkG_NormalNear_Atk3::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack3"), false, false, 1.5f);
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
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack3End"), false, false, 1.5f);
    }
    break;
    }
}

void CMorphoAtkG_NormalNear_Atk3::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Progress: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
        GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);
        MRPFSM->ClearComboLevel();
        MRPFSM->OffWeaponRTrigger();
    }
    break;
    case StateStep::End:
        break;
    }
}

void CMorphoAtkG_NormalNear_Atk3::Progress()
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
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkG_NormalNear_Atk3::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}
