#include "pch.h"
#include "CMorphoAtkG_Wait_RightSideMove.h"
#include "CMorphoFSM.h"

CMorphoAtkG_Wait_RightSideMove::CMorphoAtkG_Wait_RightSideMove()
    : m_AccTime(0.f)
    , m_SoundIdx(0)
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

    // Sound
    if ((m_SoundIdx == 1 && t >= 1.f / 3.f) || (m_SoundIdx == 2 && t >= 2.f / 3.f))
    {
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\0000_FootStep.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 5.f);
        m_SoundIdx++;
    }

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

    // Sound
    wstring Sound = L"sound\\wav\\CharaMorphoknight\\0000_FootStep.wav";
    GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 5.f);
    m_SoundIdx = 1;
}

void CMorphoAtkG_Wait_RightSideMove::Exit_Step()
{
    GetOwner()->Rigidbody()->SetVelocity(Vec3());
}