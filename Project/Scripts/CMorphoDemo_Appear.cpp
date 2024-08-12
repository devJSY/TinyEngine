#include "pch.h"
#include "CMorphoDemo_Appear.h"
#include "CMorphoFSM.h"
#include "CChangeAlphaScript.h"

CMorphoDemo_Appear::CMorphoDemo_Appear()
    : m_StartPos(Vec3(0.f, 0.f, -300.f))
    , m_DownSpeed(80.f)
    , m_AccTime(0.f)
    , m_BossName(nullptr)
{
    m_BossNamePref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\BossName_Morpho.pref", L"prefab\\BossName_Morpho.pref");
}

CMorphoDemo_Appear::~CMorphoDemo_Appear()
{
}

void CMorphoDemo_Appear::tick()
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

void CMorphoDemo_Appear::Exit()
{
    Exit_Step();

    if (m_BossName)
    {
        GamePlayStatic::DestroyGameObject(m_BossName);
    }
}

void CMorphoDemo_Appear::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DemoMorphoKnightAppearCut4"), false, false, 1.5f, 0.f);
        GetOwner()->Animator()->SetClipFrameIndex(400);
        GetOwner()->Transform()->SetWorldPos(m_StartPos);
        GetOwner()->Transform()->SetWorldRotation(Vec3());
        m_AccTime = 0.f;

        //@CAMERA : 점점 몰포 가까이
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DemoMorphoKnightAppearCut5"), false, false, 1.5f);

        // spawn BossName
        if (m_BossNamePref != nullptr)
        {
            m_BossName = m_BossNamePref->Instantiate();
            CChangeAlphaScript* Script = m_BossName->GetScript<CChangeAlphaScript>();

            m_BossName->Transform()->SetWorldPos(Vec3(0.f, 155.f, -430.f));
            m_BossName->Transform()->SetWorldRotation(Vec3());
            m_BossName->Transform()->SetWorldScale(Vec3(5.f));
            Script->FadeIn(0.5f);

            GamePlayStatic::SpawnGameObject(m_BossName, LAYER_STATIC);
        }

        //@CAMERA : 뒤로 이동(고정)
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DemoBirthEnd"), false, false, 1.5f);

        //@CAMERA : 투타겟
    }
    break;
    }
}

void CMorphoDemo_Appear::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress: {
        if (m_BossName)
        {
            CChangeAlphaScript* Script = m_BossName->GetScript<CChangeAlphaScript>();
            Script->FadeOutDestroy(1.f);
            m_BossName = nullptr;
        }
    }
    break;
    case StateStep::End:
        break;
    }
}

void CMorphoDemo_Appear::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoDemo_Appear::Progress()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoDemo_Appear::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->ChangeStateGroup(MorphoStateGroup::AtkGroundWait);
    }
}