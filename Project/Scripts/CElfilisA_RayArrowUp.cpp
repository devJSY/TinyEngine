#include "pch.h"
#include "CElfilisA_RayArrowUp.h"
#include "CElfilisFSM.h"
#include "CElfilisAirArrow.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

CElfilisA_RayArrowUp::CElfilisA_RayArrowUp()
    : m_ArrowScript{nullptr,}
    , m_AccTime(0.f)
    , m_bSpawn{false,}
    , m_bReady(false)
    , m_bStateExit(false)
    , m_NextStateGroup(ElfilisStateGroup::AirIdle)
{
    m_ArrowPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisAirArrow.pref");
}

CElfilisA_RayArrowUp::~CElfilisA_RayArrowUp()
{
}

void CElfilisA_RayArrowUp::tick()
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

void CElfilisA_RayArrowUp::Exit()
{
    Exit_Step();

    for (int i = 0; i < 7; ++i)
    {
        if (m_ArrowScript[i] && !m_bReady)
        {
           GamePlayStatic::DestroyGameObject(m_ArrowScript[i]->GetOwner());
        }
    }
}

void CElfilisA_RayArrowUp::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowReadyAir"), false);

        // spawn arrow
        if (m_ArrowPref != nullptr)
        {
            for (int i = 0; i < 7; ++i)
            {
                CGameObject* NewArrow = m_ArrowPref->Instantiate();
                m_ArrowScript[i] = NewArrow->GetScript<CElfilisAirArrow>();

                Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
                Pos.y += 150.f;
                Vec3 Front = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
                Vec3 Up = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::UP);
                Vec3 Right = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

                m_ArrowScript[i]->SetTarget(PLAYER);
                m_ArrowScript[i]->SetInitPos(Pos);
                m_ArrowScript[i]->SetInitDir(Front, Up, Right);
                m_ArrowScript[i]->SetType(ArrowType::UP);
                m_ArrowScript[i]->SetArrowIdx(i);

                GamePlayStatic::SpawnGameObject(NewArrow, LAYER_MONSTERATK);
            }
        }
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowStartAir"), false);
        m_AccTime = 0.f;
        m_bReady = false;
        for (int i = 0; i < 7; ++i)
        {
            m_bSpawn[i] = false;
        }
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowWaitAir"));
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowEndAir"), false);

        // next state group 미리 결정
        m_NextStateGroup = ELFFSM->FindNextStateGroup();
        m_bStateExit = false;
    }
    break;
    }
}

void CElfilisA_RayArrowUp::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress:
        break;
    case StateStep::Wait:
        break;
    case StateStep::End:
        break;
    }
}

void CElfilisA_RayArrowUp::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CElfilisA_RayArrowUp::Progress()
{
    m_AccTime += DT;

    // Arrow Spawn
    CastArrow();

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisA_RayArrowUp::Wait()
{
    m_AccTime += DT;

    // 이전 Step에서 처리완료되지 않은 Arrows 처리
    CastArrow();

    if (m_bReady)
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisA_RayArrowUp::End()
{
    if (m_NextStateGroup == ELFFSM->GetCurStateGroup())
    {
        if (GetOwner()->Animator()->IsFinish())
        {
            ELFFSM->RepeatState();
        }
    }
    else
    {
        if (m_bStateExit || m_ArrowScript[3]->IsAttack())
        {
            m_bStateExit = true;
            m_AccTime += DT;

            if (m_AccTime > 1.5f)
            {
                ELFFSM->ChangeStateGroup(m_NextStateGroup);
            }
        }
    }
}

void CElfilisA_RayArrowUp::CastArrow()
{
    float SpawnTime = 0.2f;

    // Arrow Spawn
    if (!m_bSpawn[0])
    {
        for (int i = 0; i < 7; ++i)
        {
            int idx = (i <= 3) ? i + 3 : 6 - i;

            if (!m_bSpawn[idx] && m_ArrowScript[idx])
            {
                if (m_AccTime >= SpawnTime * i)
                {
                    m_ArrowScript[idx]->StartSpawn();
                    m_bSpawn[idx] = true;
                }
            }
        }
    }

    // Arrow Ready
    else
    {
        if (m_ArrowScript[0]->IsSpawnFinished() && !m_bReady)
        {
            for (int i = 0; i < 7; ++i)
            {
                m_ArrowScript[i]->StartReady();
            }
            m_bReady = true;
        }
    }
}
