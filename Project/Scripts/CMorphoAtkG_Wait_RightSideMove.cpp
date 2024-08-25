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
    float PlayTime = 1.8f;
    float Speed = 2.f;
    m_AccTime += DT;

    // move
    float t = m_AccTime / PlayTime;
    float NewSpeed = Speed * fabsf(sinf(t * XM_PI * 3.f));
    Vec3 NewVeloc = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * NewSpeed;
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

void CMorphoAtkG_Wait_RightSideMove::Enter_Step()
{
    GetOwner()->Animator()->Play(ANIMPREFIX("RightSideMove"), true, false, 1.f);
    m_AccTime = 0.f;
}

void CMorphoAtkG_Wait_RightSideMove::Exit_Step()
{
    GetOwner()->Rigidbody()->SetVelocity(Vec3());
}