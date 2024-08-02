#include "pch.h"
#include "CElfilisA_RayArrowUp.h"
#include "CElfilisFSM.h"
#include "CElfilisAirArrow.h"
#include <Engine\CAssetMgr.h>
#include <Engine\CPrefab.h>

CElfilisA_RayArrowUp::CElfilisA_RayArrowUp()
    : m_Arrow{}
    , m_ArrowScript{}
    , m_AccTime(0.f)
    , m_bSpawnFinished{false,}
    , m_bReadyFinished(false)
{
    Ptr<CPrefab> ArrowPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisAirArrow.pref", L"prefab\\ElfilisAirArrow.pref");

    if (ArrowPref != nullptr)
    {
        for (int i = 0; i < 7; i++)
        {
            m_Arrow[i] = ArrowPref->Instantiate();
            CElfilisAirArrow* Script = m_Arrow[i]->GetScript<CElfilisAirArrow>();

            if (Script)
            {
                m_ArrowScript[i] = Script;
            }
        }
    }
}

CElfilisA_RayArrowUp::~CElfilisA_RayArrowUp()
{
    for (int i = 0; i < 7; ++i)
    {
        if (m_Arrow[i] && m_Arrow[i]->GetLayerIdx() == -1)
        {
            delete m_Arrow[i];
        }
    }
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

void CElfilisA_RayArrowUp::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowReadyAir"), false);

        for (int i = 0; i < 7; ++i)
        {
            if (m_ArrowScript[i] != nullptr)
            {
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

                GamePlayStatic::SpawnGameObject(m_Arrow[i], LAYER_MONSTERATK);
            }
        }
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowStartAir"), false);

        m_AccTime = 0.f;
        m_bReadyFinished = false;
        for (int i = 0; i < 7; ++i)
        {
            m_bSpawnFinished[i] = false;
        }
    }
    break;
    case StateStep::Wait: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowWaitAir"));
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("RayArrowEndAir"), false);
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
    static float SpawnTime = 0.2f;
    m_AccTime += DT;

    // Arrow Spawn
    for (int i = 0; i < 7; ++i)
    {
        int idx = (i <= 3) ? i + 3 : 6 - i;

        if (!m_bSpawnFinished[idx] && m_ArrowScript[idx])
        {
            if (m_AccTime >= SpawnTime * i)
            {
                m_ArrowScript[idx]->StartSpawn();
                m_bSpawnFinished[idx] = true;
            }
        }
    }

    // Arrow Ready
    if (!m_bReadyFinished && m_bSpawnFinished[0] && m_ArrowScript[0]->IsSpawnFinished())
    {
        for (int i = 0; i < 7; ++i)
        {
            m_ArrowScript[i]->StartReady();
        }
        m_bReadyFinished = true;
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Wait);
    }
}

void CElfilisA_RayArrowUp::Wait()
{
    static float SpawnTime = 0.2f;
    m_AccTime += DT;

    // Arrow Spawn
    for (int i = 0; i < 7; ++i)
    {
        int idx = (i <= 3) ? i + 3 : i - 4;

        if (!m_bSpawnFinished[idx] && m_ArrowScript[idx] != nullptr)
        {
            if (m_AccTime >= SpawnTime * i)
            {
                m_ArrowScript[idx]->StartSpawn();
                m_bSpawnFinished[idx] = true;
            }
        }
    }

    // Arrow Ready
    if (!m_bReadyFinished && m_bSpawnFinished[0])
    {
        for (int i = 0; i < 7; ++i)
        {
            m_ArrowScript[i]->StartReady();
        }
        m_bReadyFinished = true;
    }

    if (m_bReadyFinished)
    {
        ChangeStep(StateStep::End);
    }
}

void CElfilisA_RayArrowUp::End()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ElfilisStateGroup NextState = ELFFSM->FindNextStateGroup();

        if (NextState == ELFFSM->GetCurStateGroup())
        {
            // ELFFSM->RepeatState(L"AIR_ATKS_RAYARROW_DOWN");
        }
        else
        {
            ELFFSM->ChangeStateGroup_RandState(NextState);
        }
    }
}