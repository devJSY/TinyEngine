#include "pch.h"
#include "CElfilisA_DrawLaser.h"
#include "CElfilisFSM.h"
#include "CElfilisLaser.h"

CElfilisA_DrawLaser::CElfilisA_DrawLaser()
    : m_LaserScript(nullptr)
    , m_AccTime(0.f)
{
    m_LaserPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisLaser.pref", L"prefab\\ElfilisLaser.pref");

    m_SoundKeyLaserCharging = L"sound\\wav\\CharaBossChimera2\\0030_DimensionLaserCharge.wav";
    m_SoundKeyLaserChargingFinish = L"sound\\wav\\CharaBossChimera2\\0031_DimensionLaserChargeFinish.wav";
    m_SoundKeyLaser = L"sound\\wav\\CharaBossChimera2\\0032_DimensionLaser.wav";
    m_SoundKeyLaserWait = L"sound\\wav\\CharaBossChimera2\\0057_RecoveryWait.wav";
    m_SoundLaser = CAssetMgr::GetInst()->Load<CSound>(m_SoundKeyLaser);
    m_SoundLaserWait = CAssetMgr::GetInst()->Load<CSound>(m_SoundKeyLaserWait);
}

CElfilisA_DrawLaser::~CElfilisA_DrawLaser()
{
}

void CElfilisA_DrawLaser::tick()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        Ready();
    }
    break;
    case StateStep::Charge: {
        Charge();
    }
    break;
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

void CElfilisA_DrawLaser::Enter()
{
    m_Step = StateStep::Ready;
    Enter_Step();
}

void CElfilisA_DrawLaser::Exit()
{
    Exit_Step();

    if (m_LaserScript)
    {
        m_LaserScript->SafeDestroy();
        m_LaserScript = nullptr;
    }
}

void CElfilisA_DrawLaser::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionLaserReady"), false);
        //@Effect Â÷Â¡ ÆÄÆ¼Å¬

        // Sound
        GamePlayStatic::Play2DSound(m_SoundKeyLaserCharging, 1, SOUND_ELFILIS, true);
    }
    break;
    case StateStep::Charge: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionLaserCharge"), false);
    }
    break;
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionLaserStart"), false);

        // Sound
        GamePlayStatic::Play2DSound(m_SoundKeyLaserChargingFinish, 1, SOUND_ELFILIS, true);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionLaser"));
        m_AccTime = 0.f;

        // Laser Spawn
        if (m_LaserPref != nullptr)
        {
            CGameObject* Laser = m_LaserPref->Instantiate();
            m_LaserScript = Laser->GetScript<CElfilisLaser>();

            Vec3 NewRot = GetOwner()->Transform()->GetWorldRotation();
            NewRot.x += XMConvertToRadians(50.f);
            Laser->Transform()->SetWorldPos(GetOwner()->Transform()->GetWorldPos());
            Laser->Transform()->SetWorldRotation(NewRot);

            m_LaserScript->SetAutoPlay(true);
            GamePlayStatic::SpawnGameObject(Laser, LAYER_MONSTERATK_TRIGGER);
        }

        // Sound
        GamePlayStatic::Play2DSound(m_SoundKeyLaser, 1, SOUND_ELFILIS * 1.4f, true);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DimensionLaserEnd"), false);

        if (m_LaserScript)
        {
            m_LaserScript->SetEnd();
            m_LaserScript = nullptr;
        }
    }
    break;
    }
}

void CElfilisA_DrawLaser::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Ready:
        break;
    case StateStep::Charge:
        break;
    case StateStep::Start:
        break;
    case StateStep::Progress: {
        GamePlayStatic::StopSound(m_SoundKeyLaserWait);
    }
    break;
    case StateStep::End:
        break;
    }
}

void CElfilisA_DrawLaser::Ready()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Charge);
    }
}

void CElfilisA_DrawLaser::Charge()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Start);
    }
}

void CElfilisA_DrawLaser::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisA_DrawLaser::Progress()
{
    m_AccTime += DT;

    // rotate laser
    if (m_LaserScript && m_LaserScript->GetStep() == StateStep::Progress)
    {
        CGameObject* Laser = m_LaserScript->GetOwner();
        Vec3 NewDir = (PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos()).Normalize();
        Laser->Transform()->Slerp(NewDir, 2.f * DT);
    }

    // sound
    if (m_SoundLaser != nullptr && m_SoundLaserWait != nullptr && !m_SoundLaser->IsPlaying() && !m_SoundLaserWait->IsPlaying())
    {
        GamePlayStatic::Play2DSound(m_SoundKeyLaserWait, 0, SOUND_ELFILIS);
    }

    if (m_AccTime > 15.f)
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisA_DrawLaser::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();
        ELFFSM->ChangeStateGroup(NextState);
    }
}