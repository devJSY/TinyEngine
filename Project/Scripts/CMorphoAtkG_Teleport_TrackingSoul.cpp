#include "pch.h"
#include "CMorphoAtkG_Teleport_TrackingSoul.h"
#include "CMorphoFSM.h"
#include "CMorphoTrackingSoul.h"

CMorphoAtkG_Teleport_TrackingSoul::CMorphoAtkG_Teleport_TrackingSoul()
    : m_AccTime(0.f)
    , m_ChargeTime(1.f)
    , m_WaitTime(2.f)
{
    m_SoulPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\MorphoTrackingSoul.pref", L"prefab\\MorphoTrackingSoul.pref");
}

CMorphoAtkG_Teleport_TrackingSoul::~CMorphoAtkG_Teleport_TrackingSoul()
{
}

void CMorphoAtkG_Teleport_TrackingSoul::tick()
{
    switch (m_Step)
    {
    case StateStep::Charge: {
        Charge();
    }
    break;
    case StateStep::ChargeWait: {
        ChargeWait();
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

void CMorphoAtkG_Teleport_TrackingSoul::Enter()
{
    m_Step = StateStep::Charge;
    Enter_Step();
}

void CMorphoAtkG_Teleport_TrackingSoul::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Charge: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AttackHiChargeStart"), false, false, 1.5f);
    }
    break;
    case StateStep::ChargeWait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AttackHiCharge"), true, false, 1.5f);
        m_AccTime = 0.f;
        //@EFFECT Â÷Â¡ÀÌÆåÆ®
    }
    break;
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AttackHiStart"), false, false, 1.5f);
        //@EFFECT Â÷Â¡¿Ï·áÀÌÆåÆ®
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AttackHiWait"), true, false, 1.5f);
        m_AccTime = 0.f;

        // spawn soul
        if (m_SoulPref != nullptr)
        {
            CGameObject* pSoul = m_SoulPref->Instantiate();
            CMorphoTrackingSoul* pScript = pSoul->GetScript<CMorphoTrackingSoul>();

            Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
            Pos.y = 150.f;
            Vec3 Dir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Dir.y = 0.f;

            pSoul->Transform()->SetWorldPos(Pos);
            pSoul->Transform()->Slerp(Dir.Normalize(), 1.f);

            pScript->SetTarget(PLAYER);

            GamePlayStatic::SpawnGameObject(pSoul, LAYER_MONSTERATK);
        }
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("AttackHiEnd"), false, false, 1.5f);
    }
    break;
    }
}

void CMorphoAtkG_Teleport_TrackingSoul::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Charge:
        break;
    case StateStep::ChargeWait:
        break;
    case StateStep::Start:
        break;
    case StateStep::Wait:
        break;
    case StateStep::End:
        break;
    }
}

void CMorphoAtkG_Teleport_TrackingSoul::Charge()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::ChargeWait);
    }
}

void CMorphoAtkG_Teleport_TrackingSoul::ChargeWait()
{
    m_AccTime += DT;

    if (m_AccTime > m_ChargeTime)
    {
        ChangeStep(StateStep::Start);
    }
}

void CMorphoAtkG_Teleport_TrackingSoul::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CMorphoAtkG_Teleport_TrackingSoul::Wait()
{
    m_AccTime += DT;

    if (m_AccTime > m_WaitTime)
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkG_Teleport_TrackingSoul::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}