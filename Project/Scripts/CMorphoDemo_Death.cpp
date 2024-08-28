#include "pch.h"
#include "CMorphoDemo_Death.h"
#include "CMorphoFSM.h"
#include "CFlowMgr_BossMorpho.h"
#include "CCameraController.h"

CMorphoDemo_Death::CMorphoDemo_Death()
    : m_AccTime(0.f)
{
}

CMorphoDemo_Death::~CMorphoDemo_Death()
{
}

void CMorphoDemo_Death::tick()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        Ready();
    }
    break;
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Wait: {
        Wait();
    }
    break;
    case StateStep::End: {
        End();
    }
    break;
    }
}

void CMorphoDemo_Death::Enter()
{
    m_Step = StateStep::Ready;
    Enter_Step();
}

void CMorphoDemo_Death::Exit()
{
    Exit_Step();
    MRPFSM->ResetEmissive();
}

void CMorphoDemo_Death::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Ready: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DamageDead"), false, false, 1.5f);
        GetOwner()->Animator()->SetPlay(false);
        m_AccTime = 0.f;

        MRPFSM->SetGlobalState(true);

        // Camera : Àá±ñ¸ØÃã
        CAMERACTRL->SetLock(true, 0.5f);

        if (CBossMgr::GetMorphoFlowMgr())
        {
            CBossMgr::GetMorphoFlowMgr()->ChangeFlowDeath();
        }
    }
    break;
    case StateStep::Start: {
        MRPFSM->DestroySumon();

        GetOwner()->Animator()->SetPlay(true);
        GetOwner()->Transform()->SetWorldPos(Vec3(0.f, 0.f, -300.f));
        GetOwner()->Transform()->SetWorldRotation(Vec3());

        // CAMERA : lengthº¯ÇÔ
        CAMERACTRL->FixedView(true, Vec3(1.33f, 31.13f, -201.47f));
        CAMERACTRL->SetImmediate(true);
        CAMERACTRL->SetMainTarget(BOSS->GetChildObject(L"CameraTarget"));
        CAMERACTRL->SetRotationSpeed(30.f);
        CAMERACTRL->SetTargetOffset(Vec3(0.f, 10.f, 0.f));
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Damage2Wait"), true, false, 1.5f);
        m_AccTime = 0.f;
        //@EFFECT »Õ¾îÁ®³ª¿À´Â ºû, ¸ðÀÌ´Â ÆÄÆ¼Å¬
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("DeathFloatAll"), false, false, 1.5f);
    }
    break;
    }
}

void CMorphoDemo_Death::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Wait:
        break;
    case StateStep::End:
        break;
    }
}

void CMorphoDemo_Death::Ready()
{
    m_AccTime += DT;

    if (m_AccTime > 0.5f)
    {
        ChangeStep(StateStep::Start);
    }
}

void CMorphoDemo_Death::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CMorphoDemo_Death::Wait()
{
    m_AccTime += DT;

    // emissive
    float ProgressTime = 1.f;
    float t1 = m_AccTime / ProgressTime;
    float t2 = sinf(t1 * XM_PI / 2.f);
    Vec3 Color = Vec3(t2, t1, t1);

    MRPFSM->AddEmissive(Color);

    if (m_AccTime > ProgressTime)
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoDemo_Death::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        //@EFFECT ÅÍÁö´Â ÆÄÆ¼Å¬

        if (CBossMgr::GetMorphoFlowMgr())
        {
            MRPFSM->SetGlobalState(false);
            CBossMgr::GetMorphoFlowMgr()->ChangeFlowClear();
        }
    }
}