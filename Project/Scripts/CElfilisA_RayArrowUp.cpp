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
    , m_ArrowGroup(0)
    , m_bSpawn{false,}
    , m_bReady(false)
{
    Ptr<CPrefab> ArrowPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\ElfilisAirArrow.pref", L"prefab\\ElfilisAirArrow.pref");

    if (ArrowPref != nullptr)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int i = 0; i < 7; i++)
            {
                m_Arrow[j][i] = ArrowPref->Instantiate();
                CElfilisAirArrow* Script = m_Arrow[j][i]->GetScript<CElfilisAirArrow>();

                if (Script)
                {
                    m_ArrowScript[j][i] = Script;
                }
            }
        }
    }
}

CElfilisA_RayArrowUp::~CElfilisA_RayArrowUp()
{
    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < 7; ++i)
        {
            // if (m_Arrow[i] && m_Arrow[i]->GetLayerIdx() == -1)
            if (m_Arrow[j][i])
            {
                delete m_Arrow[j][i];
            }
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

        // check spawn activation
        m_ArrowGroup = 0;

        for (int i = 0; i < 7; i++)
        {
            bool ret = false;

            if (m_Arrow[0][i]->IsActive())
            {
                ret = true;
            }

            if (ret)
            {
                m_ArrowGroup = 1;
                break;
            }
        }

        // spawn arrow
        for (int i = 0; i < 7; ++i)
        {
            if (m_ArrowScript[m_ArrowGroup][i] != nullptr)
            {
                Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
                Pos.y += 150.f;
                Vec3 Front = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
                Vec3 Up = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::UP);
                Vec3 Right = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

                m_ArrowScript[m_ArrowGroup][i]->SetTarget(PLAYER);
                m_ArrowScript[m_ArrowGroup][i]->SetInitPos(Pos);
                m_ArrowScript[m_ArrowGroup][i]->SetInitDir(Front, Up, Right);
                m_ArrowScript[m_ArrowGroup][i]->SetType(ArrowType::UP);
                m_ArrowScript[m_ArrowGroup][i]->SetArrowIdx(i);

                GamePlayStatic::SpawnGameObject(m_Arrow[m_ArrowGroup][i], LAYER_MONSTERATK);
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
            m_bSpawn[m_ArrowGroup][i] = false;
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

    if (!m_bSpawn[m_ArrowGroup][0])
    {
        // Arrow Spawn
        for (int i = 0; i < 7; ++i)
        {
            int idx = (i <= 3) ? i + 3 : 6 - i;

            if (!m_bSpawn[m_ArrowGroup][idx] && m_ArrowScript[m_ArrowGroup][idx])
            {
                if (m_AccTime >= SpawnTime * i)
                {
                    m_ArrowScript[m_ArrowGroup][idx]->StartSpawn();
                    m_bSpawn[m_ArrowGroup][idx] = true;
                }
            }
        }
    }
    else
    {
        // Arrow Ready
        if (m_ArrowScript[m_ArrowGroup][0]->IsSpawnFinished() && !m_bReady)
        {
            for (int i = 0; i < 7; ++i)
            {
                m_ArrowScript[m_ArrowGroup][i]->StartReady();
            }
            m_bReady = true;
        }
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

    if (!m_bSpawn[m_ArrowGroup][0])
    {
        // Arrow Spawn
        for (int i = 0; i < 7; ++i)
        {
            int idx = (i <= 3) ? i + 3 : 6 - i;

            if (!m_bSpawn[m_ArrowGroup][idx] && m_ArrowScript[m_ArrowGroup][idx])
            {
                if (m_AccTime >= SpawnTime * i)
                {
                    m_ArrowScript[m_ArrowGroup][idx]->StartSpawn();
                    m_bSpawn[m_ArrowGroup][idx] = true;
                }
            }
        }
    }
    else
    {
        // Arrow Ready
        if (m_ArrowScript[m_ArrowGroup][0]->IsSpawnFinished() && !m_bReady)
        {
            for (int i = 0; i < 7; ++i)
            {
                m_ArrowScript[m_ArrowGroup][i]->StartReady();
            }
            m_bReady = true;
        }
    }

    if (m_bReady)
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
            ELFFSM->RepeatState();
        }
        else
        {
            ELFFSM->ChangeStateGroup(NextState);
        }
    }
}