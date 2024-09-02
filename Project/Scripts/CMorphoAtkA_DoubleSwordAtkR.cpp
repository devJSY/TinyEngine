#include "pch.h"
#include "CMorphoAtkA_DoubleSwordAtkR.h"
#include "CMorphoFSM.h"
#include "CPlayerMgr.h"
#include "CCameraController.h"
#include "CChangeAlphaScript.h"
#include "CDestroyParticleScript.h"

CMorphoAtkA_DoubleSwordAtkR::CMorphoAtkA_DoubleSwordAtkR()
    : m_LightningEffect(nullptr)
    , m_bFrmEnter(true)
{
}

CMorphoAtkA_DoubleSwordAtkR::~CMorphoAtkA_DoubleSwordAtkR()
{
}

void CMorphoAtkA_DoubleSwordAtkR::tick()
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

void CMorphoAtkA_DoubleSwordAtkR::Exit()
{
    Exit_Step();
    MRPFSM->OffWeaponRTrigger();
}

void CMorphoAtkA_DoubleSwordAtkR::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack1Start"), false, false, 1.5f);
        MRPFSM->OnWeaponRTrigger();
        //@EFFECT Â÷Â¡ÀÌÆåÆ®
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack1"), false, false, 1.5f);
        m_bFrmEnter = true;
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack1End"), false, false, 1.5f);
        m_bFrmEnter = true;
    }
    break;
    }
}

void CMorphoAtkA_DoubleSwordAtkR::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::End: {
        m_LightningEffect = nullptr;
    }
    break;
    }
}

void CMorphoAtkA_DoubleSwordAtkR::Start()
{
    RotateToPlayer(DT);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkA_DoubleSwordAtkR::Progress()
{
    // Spawn Effect
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 30))
    {
        m_bFrmEnter = false;

        Vec3 Pos = MRPFSM->GetWeaponR()->GetChildObject()[0]->Transform()->GetWorldPos();
        Pos.y = 0.f;
        m_LightningEffect = MRPFSM->GetUnit()->SpawnLightningEffect(Pos);
        MRPFSM->GetUnit()->SpawnCircleDustEffect(Pos);

        CAMERACTRL->Shake(0.3f, 30.f, 30.f);

        // Spawn DropStar & Particle Butterfly
        Vec3 SpawnPos = MRPFSM->GetWeaponR()->Transform()->GetWorldPos();
        Vec3 FrontDir = MRPFSM->GetWeaponR()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
        Vec3 RightDir = MRPFSM->GetWeaponR()->Transform()->GetWorldDir(DIR_TYPE::UP);
        FrontDir.y = RightDir.y = 0.f;
        FrontDir.y = 0.f;
        FrontDir.Normalize();
        RightDir.Normalize();

        for (int i = 0; i < 3; ++i)
        {
            Vec3 OffsetSpawnPos = SpawnPos;
            OffsetSpawnPos += RightDir * 20.f;
            OffsetSpawnPos += FrontDir * (100.f * i);

            MRPFSM->GetUnit()->SpawnAttackButterflyEffect(OffsetSpawnPos);
            MRPFSM->SpawnDropStar(OffsetSpawnPos);
        }

        for (int i = 0; i < 3; ++i)
        {
            Vec3 OffsetSpawnPos = SpawnPos;
            OffsetSpawnPos -= RightDir * 20.f;
            OffsetSpawnPos += FrontDir * (50.f + 100.f * i);

            MRPFSM->GetUnit()->SpawnAttackButterflyEffect(OffsetSpawnPos);
            MRPFSM->SpawnDropStar(OffsetSpawnPos);
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkA_DoubleSwordAtkR::End()
{
    // ÀÌÆåÆ® ÇØÁ¦
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 15))
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