#include "pch.h"
#include "CMorphoAtkA_DoubleSwordAtkL.h"
#include "CMorphoFSM.h"
#include "CPlayerMgr.h"
#include "CCameraController.h"
#include "CChangeAlphaScript.h"

CMorphoAtkA_DoubleSwordAtkL::CMorphoAtkA_DoubleSwordAtkL()
    : m_LightningEffect(nullptr)
    , m_bFrmEnter(true)
{
    m_LightningEffectPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Effect_MorphoLightningSet.pref", L"prefab\\Effect_MorphoLightningSet.pref");
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
}

void CMorphoAtkA_DoubleSwordAtkL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack2Start"), false, false, 1.5f);
        MRPFSM->OnWeaponLTrigger();
        //@EFFECT 차징이펙트
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack2"), false, false, 1.5f);
        m_bFrmEnter = true;
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DoubleSwordAttack2End"), false, false, 1.5f);
        m_bFrmEnter = true;
    }
    break;
    }
}

void CMorphoAtkA_DoubleSwordAtkL::Exit_Step()
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
    //@EFFECT 번개, 나비, 충격파
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 30))
    {
        m_bFrmEnter = false;

        if (m_LightningEffectPref != nullptr)
        {
            m_LightningEffect = m_LightningEffectPref->Instantiate();
            Vec3 Pos = MRPFSM->GetWeaponL()->GetChildObject()[0]->Transform()->GetWorldPos();
            Pos.y = 0.f;
            Vec3 Dir = CPlayerMgr::GetCameraController()->GetLookDir() * -1.f;
            Dir.y = 0.f;

            m_LightningEffect->Transform()->SetWorldPos(Pos);
            m_LightningEffect->Transform()->Slerp(Dir.Normalize(), 1.f);

            GamePlayStatic::SpawnGameObject(m_LightningEffect, LAYER_EFFECT);
        }

        CAMERACTRL->Shake(0.3f, 30.f, 30.f);

        // Spawn DropStar
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
            OffsetSpawnPos += RightDir * 20.f;
            OffsetSpawnPos += FrontDir * (100.f * i);

            MRPFSM->SpawnDropStar(OffsetSpawnPos);
        }

        for (int i = 0; i < 3; ++i)
        {
            Vec3 OffsetSpawnPos = SpawnPos;
            OffsetSpawnPos -= RightDir * 20.f;
            OffsetSpawnPos += FrontDir * (50.f + 100.f * i);

            MRPFSM->SpawnDropStar(OffsetSpawnPos);
        }
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkA_DoubleSwordAtkL::End()
{
    // 이펙트 해제
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