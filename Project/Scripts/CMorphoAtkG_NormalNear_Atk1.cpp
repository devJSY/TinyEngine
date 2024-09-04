#include "pch.h"
#include "CMorphoAtkG_NormalNear_Atk1.h"
#include "CMorphoFSM.h"
#include "CCameraController.h"

CMorphoAtkG_NormalNear_Atk1::CMorphoAtkG_NormalNear_Atk1()
    : m_ComboLevel(0)
{
}

CMorphoAtkG_NormalNear_Atk1::~CMorphoAtkG_NormalNear_Atk1()
{
}

void CMorphoAtkG_NormalNear_Atk1::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Charge: {
        Charge();
    }
    break;
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

void CMorphoAtkG_NormalNear_Atk1::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1Start"), false, false, 1.5f);

        // Combo Level 사전결정
        float Rand = GetRandomfloat(1.f, 100.f);

        if (Rand <= 50.f)
        {
            m_ComboLevel = 2;
        }
        else if (Rand <= 75.f)
        {
            m_ComboLevel = 1;
        }
        else
        {
            m_ComboLevel = 0;
        }
    }
    break;
    case StateStep::Charge: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1Charge"), false, false, 1.5f);

        // Sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\Charging1.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1"), false, false, 1.5f);
        MRPFSM->OnWeaponRTrigger();

        // move dir
        m_ForceDir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
        m_ForceDir.y = 0.f;
        m_ForceDir.Normalize();

        // Sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\Swing0.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO);

        CAMERACTRL->Shake(0.3f, 20.f, 20.f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1End"), false, false, 1.5f);
    }
    break;
    }
}

void CMorphoAtkG_NormalNear_Atk1::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Charge:
        break;
    case StateStep::Progress: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
        GetOwner()->Rigidbody()->SetAngularVelocity(Vec3());

        if (m_ComboLevel == 0)
        {
            MRPFSM->ClearComboLevel();
        }
        else
        {
            GetOwner()->Rigidbody()->SetVelocity(Vec3());
            MRPFSM->SetComboLevel(m_ComboLevel);
        }

        MRPFSM->OffWeaponRTrigger();

        // Spawn DropStar
        Vec3 SpawnPos = GetOwner()->Transform()->GetWorldPos();
        SpawnPos -= GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT) * 50.f;
        MRPFSM->SpawnDropStar(SpawnPos);
    }
    break;
    case StateStep::End: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
    }
    break;
    }
}

void CMorphoAtkG_NormalNear_Atk1::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        if (m_ComboLevel == 0)
        {
            ChangeStep(StateStep::Progress);
        }
        else
        {
            ChangeStep(StateStep::Charge);
        }
    }
}

void CMorphoAtkG_NormalNear_Atk1::Charge()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkG_NormalNear_Atk1::Progress()
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
        if (m_ComboLevel == 0)
        {
            ChangeStep(StateStep::End);
        }
        else
        {
            MRPFSM->ChangeState(L"ATKG_NORMALNEAR_ATK2");
        }
    }
}

void CMorphoAtkG_NormalNear_Atk1::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}