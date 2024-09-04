#include "pch.h"
#include "CElfilisBig_SwingFinishL.h"
#include "CElfilisFSM.h"
#include "CElfilisBigFSM.h"
#include "CMomentaryObjScript.h"
#include "CCameraController.h"

CElfilisBig_SwingFinishL::CElfilisBig_SwingFinishL()
    : m_bFrmEnter(true)
    , m_AccTime(0.f)
    , m_PrevAlpha(1.f)
{
    m_Rock = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisMapRock.pref", L"prefab\\ElfilisMapRock.pref");
}

CElfilisBig_SwingFinishL::~CElfilisBig_SwingFinishL()
{
}

void CElfilisBig_SwingFinishL::tick()
{
    switch (m_Step)
    {
    case StateStep::Wait: {
        Wait();
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
    case StateStep::EndWait: {
        EndWait();
    }
    break;
    }
}

void CElfilisBig_SwingFinishL::Enter()
{
    m_Step = StateStep::Wait;
    Enter_Step();

    ELFBIGFSM->ClearComboLevel();
}

void CElfilisBig_SwingFinishL::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Wait: {
        // GetOwner()->Animator()->Play(ANIMPREFIX("BigChainRL"), false, false, 4.f);
        ChangeStep(StateStep::Progress);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("BigSwingFinishLeft"), false, false, 4.f, 1.f);
        m_bFrmEnter = true;
        m_SoundIdx = 0;

        // sound
        wstring Wind = L"sound\\wav\\CharaBossChimera2\\0037_BigSwing.wav";
        GamePlayStatic::Play2DSound(Wind, 1, SOUND_ELFILIS);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("BigSwingFinishLeftEnd"), false, false, 4.f);
    }
    break;
    case StateStep::EndWait: {
        m_AccTime = 0.f;
        m_PrevAlpha = *(float*)(GetOwner()->MeshRender()->GetMaterial(0)->GetScalarParam(SCALAR_PARAM::FLOAT_2));
    }
    break;
    }
}

void CElfilisBig_SwingFinishL::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Wait:
        break;
    case StateStep::Progress:
        break;
    case StateStep::End:
        break;
    case StateStep::EndWait:
        break;
    }
}

void CElfilisBig_SwingFinishL::Wait()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisBig_SwingFinishL::Progress()
{
    // Spawn Rock
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 490))
    {
        if (m_Rock != nullptr)
        {
            CGameObject* pRock = m_Rock->Instantiate();
            CMomentaryObjScript* pScript = pRock->GetScript<CMomentaryObjScript>();

            // pScript->AddEffect(MomentaryEffectType::AppearScaling);
            pScript->SetPlayTime(3.5f);
            GamePlayStatic::SpawnGameObject(pRock, LAYER_DYNAMIC);
        }

        // Camera Shake
        CAMERACTRL->Shake(0.3f, 15.f, 120.f);

        m_bFrmEnter = false;
    }

    // Elfilis ¶¥À¸·Î µ¹¾Æ¿È
    if (ELFFSM->IsPattern(ElfilisPatternType::BigCombo, 6) || ELFFSM->IsPattern(ElfilisPatternType::Appear2, 3))
    {
        if (CHECK_ANIMFRM(GetOwner(), 600))
        {
            ELFFSM->ProcPatternStep();
            ChangeStep(StateStep::EndWait);
        }
    }

    // sound
    if (m_SoundIdx == 0 && CHECK_ANIMFRM(GetOwner(), 400))
    {
        m_SoundIdx++;

        // sound
        wstring Wind = L"sound\\wav\\CharaBossChimera2\\0045_SlashWind.wav";
        GamePlayStatic::Play2DSound(Wind, 1, SOUND_ELFILIS * 2.f);
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisBig_SwingFinishL::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::EndWait);
    }
}

void CElfilisBig_SwingFinishL::EndWait()
{
    float FadeTime = 1.f;
    m_AccTime += DT;

    if (m_AccTime <= FadeTime)
    {
        float t = m_PrevAlpha - (m_AccTime / FadeTime);
        ELFFSM->GetBigBossFSM()->SetMtrlTransparent(t);
    }
    else
    {
        GetOwner()->SetActive(false);
    }
}
