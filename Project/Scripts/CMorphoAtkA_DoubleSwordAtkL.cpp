#include "pch.h"
#include "CMorphoAtkA_DoubleSwordAtkL.h"
#include "CMorphoFSM.h"
#include "CMorphoUnit.h"
#include "CPlayerMgr.h"
#include "CCameraController.h"
#include "CChangeAlphaScript.h"
#include "CDestroyParticleScript.h"

CMorphoAtkA_DoubleSwordAtkL::CMorphoAtkA_DoubleSwordAtkL()
    : m_LightningEffect(nullptr)
    , m_bFrmEnter(true)
{
}

CMorphoAtkA_DoubleSwordAtkL::~CMorphoAtkA_DoubleSwordAtkL()
{
}

void CMorphoAtkA_DoubleSwordAtkL::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
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

void CMorphoAtkA_DoubleSwordAtkL::Exit()
{
    Exit_Step();

    MRPFSM->OffWeaponLTrigger();
    MRPFSM->OffWeaponRTrigger();
}

void CMorphoAtkA_DoubleSwordAtkL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack2Start"), false, false, 1.5f);
        MRPFSM->OnWeaponLTrigger();
        MRPFSM->OnWeaponRTrigger();
        //@EFFECT Â÷Â¡ÀÌÆåÆ®
        
        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordCharging.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 0.9f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack2"), false, false, 1.5f);
        m_bFrmEnter = true;

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordAttackStart.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 0.8f);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack2End"), false, false, 1.5f);
        m_bFrmEnter = true;

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordAttackStart.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 0.4f);
    }
    break;
    }
}

void CMorphoAtkA_DoubleSwordAtkL::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        // sound
        wstring SoundCharging = L"sound\\wav\\CharaMorphoknight\\DoubleSwordCharging.wav";
        CSoundMgr::GetInst()->FadeSound(SoundCharging, SOUND_MORPHO * 0.9f, 0.f, 0.5f, false);
    }
        break;
    case StateStep::Progress:
        break;
    case StateStep::End: {
        m_LightningEffect = nullptr;
    }
    break;
    }
}

void CMorphoAtkA_DoubleSwordAtkL::Start()
{
    RotateToPlayer(DT);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkA_DoubleSwordAtkL::Progress()
{
    // Spawn Effect
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 30))
    {
        m_bFrmEnter = false;

        Vec3 Pos = MRPFSM->GetWeaponL()->GetChildObject()[0]->Transform()->GetWorldPos();
        Pos.y = 0.f;
        m_LightningEffect = MRPFSM->GetUnit()->SpawnLightningEffect(Pos);
        MRPFSM->GetUnit()->SpawnCircleDustEffect(Pos);

        CAMERACTRL->Shake(0.3f, 30.f, 30.f);

        // Spawn DropStar & Particle Butterfly
        Vec3 SpawnPos = MRPFSM->GetWeaponL()->Transform()->GetWorldPos();
        Vec3 FrontDir = MRPFSM->GetWeaponL()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 RightDir = MRPFSM->GetWeaponL()->Transform()->GetWorldDir(DIR_TYPE::UP);
        FrontDir.y = RightDir.y = 0.f;
        FrontDir.y = 0.f;
        FrontDir.Normalize();
        RightDir.Normalize();

        for (int i = 0; i < 3; ++i)
        {
            Vec3 OffsetSpawnPos = SpawnPos;
            OffsetSpawnPos += FrontDir * (100.f * i);
            OffsetSpawnPos += RightDir * 20.f;

            MRPFSM->GetUnit()->SpawnAttackButterflyEffect(OffsetSpawnPos);
            MRPFSM->SpawnDropStar(OffsetSpawnPos);
        }

        for (int i = 0; i < 3; ++i)
        {
            Vec3 OffsetSpawnPos = SpawnPos;
            OffsetSpawnPos += FrontDir * (50.f + 100.f * i);
            OffsetSpawnPos -= RightDir * 20.f;

            MRPFSM->GetUnit()->SpawnAttackButterflyEffect(OffsetSpawnPos);
            MRPFSM->SpawnDropStar(OffsetSpawnPos);
        }

        // sound
        wstring Sound = L"sound\\wav\\CharaMorphoknight\\DoubleSwordAttackCollision.wav";
        GamePlayStatic::Play2DSound(Sound, 1, SOUND_MORPHO * 1.5f);
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkA_DoubleSwordAtkL::End()
{
    // ÀÌÆåÆ® ÇØÁ¦
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 10))
    {
        m_bFrmEnter = false;

        if (m_LightningEffect)
        {
            CChangeAlphaScript* Script = m_LightningEffect->GetScript<CChangeAlphaScript>();
            Script->FadeOutDestroy(1.f);
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->ProcPatternStep();
    }
}