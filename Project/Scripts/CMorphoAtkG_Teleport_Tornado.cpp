#include "pch.h"
#include "CMorphoAtkG_Teleport_Tornado.h"
#include "CMorphoFSM.h"

CMorphoAtkG_Teleport_Tornado::CMorphoAtkG_Teleport_Tornado()
    : m_TornadoSpeed(100.f)
{
}

CMorphoAtkG_Teleport_Tornado::~CMorphoAtkG_Teleport_Tornado()
{
}

void CMorphoAtkG_Teleport_Tornado::tick()
{
    switch (m_Step)
    {
    case StateStep::Charge: {
        Charge();
    }
    break;
    case StateStep::ChargeWait: {
        Start();
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

void CMorphoAtkG_Teleport_Tornado::Enter()
{
    m_Step = StateStep::Charge;
    Enter_Step();
}

void CMorphoAtkG_Teleport_Tornado::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Charge: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TornadoAttackCharge"), false, false, 1.5f);
    }
    break;
    case StateStep::ChargeWait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TornadoAttackChargeMax"), true, false, 1.5f);
    }
    break;
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TornadoAttackStart"), false, false, 1.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TornadoAttackWait"), true, false, 1.5f);

        //@TODO 회오리 추가
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("TornadoAttackEnd"), false, false, 1.5f);
    }
    break;
    }
}

void CMorphoAtkG_Teleport_Tornado::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Charge:
        break;
    case StateStep::ChargeWait:
        break;
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::End:
        break;
    }
}

void CMorphoAtkG_Teleport_Tornado::Charge()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::ChargeWait);
    }
}

void CMorphoAtkG_Teleport_Tornado::ChargeWait()
{
    //@TODO 차징이펙트 끝나고
}

void CMorphoAtkG_Teleport_Tornado::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkG_Teleport_Tornado::Progress()
{
    //@TODO 회오리 다지나가고
}

void CMorphoAtkG_Teleport_Tornado::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}