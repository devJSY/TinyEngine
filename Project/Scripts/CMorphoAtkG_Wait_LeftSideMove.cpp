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
    float PlayTime = 1.8f;
    float Speed = 2.f;
    m_AccTime += DT;

    // move
    float t = m_AccTime / PlayTime;
    float NewSpeed = Speed * fabsf(sinf(t * XM_PI * 3.f));
    Vec3 NewVeloc = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * -NewSpeed;
    GetOwner()->Rigidbody()->SetVelocity(NewVeloc);

    // change state
    if (m_AccTime > 1.5f)
    {
        float Rand = GetRandomfloat(1.f, 10.f);

        if (Rand <= 5.f)
        {
            MRPFSM->Attack();
        }
        else
        {
            MRPFSM->Move();
        }
    }
}

void CMorphoAtkG_Wait_LeftSideMove::Enter_Step()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("LeftSideMove"), true, false, 1.f);
    GetOwner()->Rigidbody()->SetVelocity(Vec3());
    GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());
    m_AccTime = 0.f;
}

void CMorphoAtkG_Wait_LeftSideMove::Exit_Step()
{
    GetOwner()->Rigidbody()->SetVelocity(Vec3());
}