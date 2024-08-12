#include "pch.h"
#include "CMorphoAtkG_NormalNear_Atk1.h"
#include "CMorphoFSM.h"

CMorphoAtkG_NormalNear_Atk1::CMorphoAtkG_NormalNear_Atk1()
    : m_ComboLevel(0)
    , m_PrevDrag(0.f)
{
}

CMorphoAtkG_NormalNear_Atk1::~CMorphoAtkG_NormalNear_Atk1()
{
}

void CMorphoAtkG_NormalNear_Atk1::tick()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        Start();
    }
    break;
    case StateStep::Charge: {
        Charge();
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

void CMorphoAtkG_NormalNear_Atk1::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1Start"), false, false, 1.5f);

        // Combo Level 사전결정
        float Rand = GetRandomfloat(1.f, 100.f);

        if (Rand <= 50.f)
        {
            m_ComboLevel = 2;
        }
        else if (Rand <= 75.f)
        {
            m_ComboLevel = 1;
        }
        else
        {
            m_ComboLevel = 0;
        }
    }
    break;
    case StateStep::Charge: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1Charge"), false, false, 1.5f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1"), false, false, 1.5f);

        // move
        Vec3 Dir = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
        Dir.y = 0.f;
        Dir.Normalize();

        m_PrevDrag = GetOwner()->Rigidbody()->GetDrag();
        GetOwner()->Rigidbody()->AddForce(Dir * 30.f, ForceMode::Impulse);
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("Attack1End"), false, false, 1.5f);
    }
    break;
    }
}

void CMorphoAtkG_NormalNear_Atk1::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Charge:
        break;
    case StateStep::Progress: {
        GetOwner()->Rigidbody()->SetDrag(m_PrevDrag);

        if (m_ComboLevel == 0)
        {
            MRPFSM->ClearComboLevel();
        }
        else
        {
            GetOwner()->Rigidbody()->SetVelocity(Vec3());
            MRPFSM->SetComboLevel(m_ComboLevel);
        }
    }
    break;
    case StateStep::End: {
        GetOwner()->Rigidbody()->SetVelocity(Vec3());
    }
    break;
    }
}

void CMorphoAtkG_NormalNear_Atk1::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        if (m_ComboLevel == 0)
        {
            ChangeStep(StateStep::Progress);
        }
        else
        {
            ChangeStep(StateStep::Charge);
        }
    }
}

void CMorphoAtkG_NormalNear_Atk1::Charge()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkG_NormalNear_Atk1::Progress()
{
    RotateToPlayer();

    // Add drag
    float t = GetOwner()->Animator()->GetClipPlayRatio();
    float Ratio = t * XM_PI / 2.f;
    float NewDrag = 4.f * sinf(Ratio);
    GetOwner()->Rigidbody()->SetDrag(NewDrag);

    // Change Step
    if (GetOwner()->Animator()->IsFinish())
    {
        if (m_ComboLevel == 0)
        {
            ChangeStep(StateStep::End);
        }
        else
        {
            MRPFSM->ChangeState(L"ATKG_NORMALNEAR_ATK2");
        }
    }
}

void CMorphoAtkG_NormalNear_Atk1::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}