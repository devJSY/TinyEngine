#include "pch.h"
#include "CMorphoAtkG_Wait_RightSideMove.h"
#include "CMorphoFSM.h"

CMorphoAtkG_Wait_RightSideMove::CMorphoAtkG_Wait_RightSideMove()
    : m_AccTime(0.f)
{
}

CMorphoAtkG_Wait_RightSideMove::~CMorphoAtkG_Wait_RightSideMove()
{
}

void CMorphoAtkG_Wait_RightSideMove::tick()
{
    m_AccTime += DT;

    // move
    float Speed = 20.f;
    Vec3 Force = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * Speed;
    GetOwner()->Rigidbody()->AddForce(Force, ForceMode::Force);

    if (GetOwner()->Rigidbody()->GetVelocity().Length() > Speed)
    {
        GetOwner()->Rigidbody()->SetVelocity(Force);
    }

    // change state
    if (m_AccTime > 1.5f)
    {
        MRPFSM->Attack();
    }
}

void CMorphoAtkG_Wait_RightSideMove::Enter_Step()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("RightSideMove"), true, false, 1.f);
    m_AccTime = 0.f;
}

void CMorphoAtkG_Wait_RightSideMove::Exit_Step()
{
    GetOwner()->Rigidbody()->SetVelocity(Vec3());
}