#include "pch.h"
#include "CMorphoAtkA_ShockWave.h"
#include "CMorphoFSM.h"

CMorphoAtkA_ShockWave::CMorphoAtkA_ShockWave()
    : m_AccTime(0.f)
    , m_ChargeTime(1.5f)
    , m_WaitTime(1.f)
{
}

CMorphoAtkA_ShockWave::~CMorphoAtkA_ShockWave()
{
}

void CMorphoAtkA_ShockWave::tick()
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

void CMorphoAtkA_ShockWave::Enter()
{
    m_Step = StateStep::Charge;
    Enter_Step();
}

void CMorphoAtkA_ShockWave::Exit()
{
    // shockwave
    const vector<CGameObject*>& vecShockWave = MRPFSM->GetShockWave();
    for (CGameObject* iter : vecShockWave)
    {
        iter->SetActive(false);
        //@EFFECT 충격파 이펙트
    }

    GetOwner()->Rigidbody()->SetUseGravity(true);

    Exit_Step();
}

void CMorphoAtkA_ShockWave::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Charge: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ButterflyScalesChargeStart"), false, false, 1.5f);
        GetOwner()->Rigidbody()->SetUseGravity(false);
    }
    break;
    case StateStep::ChargeWait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ButterflyScalesCharge"), true, false, 1.5f);
        m_AccTime = 0.f;
    }
    break;
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ButterflyScalesAttackStart"), false, false, 1.5f);

        // shockwave
        const vector<CGameObject*>& vecShockWave = MRPFSM->GetShockWave();
        for (CGameObject* iter : vecShockWave)
        {
            iter->SetActive(true);
            //@EFFECT 충격파 이펙트
        }
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ButterflyScalesAttack"), true, false, 1.5f);
        m_AccTime = 0.f;
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("ButterflyScalesAttackEnd"), false, false, 1.5f);
    }
    break;
    }
}

void CMorphoAtkA_ShockWave::Exit_Step()
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

void CMorphoAtkA_ShockWave::Charge()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::ChargeWait);
    }
}

void CMorphoAtkA_ShockWave::ChargeWait()
{
    m_AccTime += DT;

    if (m_AccTime > m_ChargeTime)
    {
        ChangeStep(StateStep::Start);
    }
}

void CMorphoAtkA_ShockWave::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CMorphoAtkA_ShockWave::Wait()
{
    m_AccTime += DT;

    if (m_AccTime > m_WaitTime)
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkA_ShockWave::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        //@TODO State전환
        MRPFSM->Move();
        //MRPFSM->ChangeStateGroup(MorphoStateGroup::MoveToGround, L"MOVEG_JUMP_FALL");
    }
}

