#include "pch.h"
#include "CMorphoAtkG_Wait_LeftSideMove.h"
#include "CMorphoFSM.h"

CMorphoAtkG_Wait_LeftSideMove::CMorphoAtkG_Wait_LeftSideMove()
    : m_AccTime(0.f)
{
}

CMorphoAtkG_Wait_LeftSideMove::~CMorphoAtkG_Wait_LeftSideMove()
{
}

void CMorphoAtkG_Wait_LeftSideMove::tick()
{
    m_AccTime += DT;

    // move
    float Speed = 20.f;
    Vec3 Force = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -Speed;
    GetOwner()->Rigidbody()->AddForce(Force, ForceMode::Force);

    if (GetOwner()->Rigidbody()->GetVelocity().Length() > Speed)
    {
        GetOwner()->Rigidbody()->SetVelocity(Force);
    }

    // change state
    if (m_AccTime > 1.3f)
    {
        MRPFSM->Attack();
    }
}

void CMorphoAtkG_Wait_LeftSideMove::Enter_Step()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("LeftSideMove"), true, false, 1.5f);
    m_AccTime = 0.f;
}

void CMorphoAtkG_Wait_LeftSideMove::Exit_Step()
{
    GetOwner()->Rigidbody()->SetVelocity(Vec3());
}