#include "pch.h"
#include "CMorphoAtkG_NormalNear_Atk3.h"
#include "CMorphoFSM.h"
#include "CCameraController.h"

CMorphoAtkG_NormalNear_Atk3::CMorphoAtkG_NormalNear_Atk3()
    : m_bFrmEnter(true)
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
        m_bFrmEnter = true;

        // move dir
        m_ForceDir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
        m_ForceDir.y = 0.f;
        m_ForceDir.Normalize();

        CAMERACTRL->Shake(0.3f, 20.f, 20.f);
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
        MRPFSM->ClearComboLevel();
        MRPFSM->OffWeaponRTrigger();

        // Spawn DropStar
        Vec3 SpawnPos = GetOwner()->Transform()->GetWorldPos();
        SpawnPos -= GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 50.f;
        MRPFSM->SpawnDropStar(SpawnPos);
    }
    break;
    case StateStep::End:
        break;
    }
}

void CMorphoAtkG_NormalNear_Atk3::Progress()
{
    RotateToPlayer();
    
    // move
    float t = GetOwner()->Animator()->GetClipPlayRatio();
    float NewSpeed = 15.f * sinf(t * XM_PI / 2.f);
    Vec3 NewVeloc = m_ForceDir * NewSpeed;
    GetOwner()->Rigidbody()->SetVelocity(NewVeloc);

    // sound
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 10))
    {
        m_bFrmEnter = false;

        // Sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\Swing1.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO);
    }

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
