#include "pch.h"
#include "CMorphoAtkG_Teleport_FireWall.h"
#include "CMorphoFSM.h"
#include "CCameraController.h"

CMorphoAtkG_Teleport_FireWall::CMorphoAtkG_Teleport_FireWall()
    : m_FireWall(nullptr)
    , m_WallSpeed(150.f)
    , m_AccTime(0.f)
    , m_bFrmEnter(true)
    , m_bSpawnDropStar(false)
{
    m_FireWallPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\MorphoFireWall.pref", L"prefab\\MorphoFireWall.pref");
}

CMorphoAtkG_Teleport_FireWall::~CMorphoAtkG_Teleport_FireWall()
{
}

void CMorphoAtkG_Teleport_FireWall::tick()
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

void CMorphoAtkG_Teleport_FireWall::Exit()
{
    Exit_Step();

    if (m_Step == StateStep::Progress)
    {
        GamePlayStatic::DestroyGameObject(m_FireWall);
        m_FireWall = nullptr;
    }
}

void CMorphoAtkG_Teleport_FireWall::Enter_Step()
{
    switch (m_Step)
    {
    case StateStep::Start: {
        GetOwner()->Animator()->Play(ANIMPREFIX("GigaMoonCharge"), false, false, 2.f);
    }
    break;
    case StateStep::Progress: {
        GetOwner()->Animator()->Play(ANIMPREFIX("GigaMoonShotComb1"), false, false, 2.f);
        MRPFSM->OnWeaponRTrigger();
        m_bFrmEnter = true;
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("GigaMoonShotCombWait"), true, false, 1.5f);
        m_AccTime = 0.f;
        m_bSpawnDropStar = false;
    }
    break;
    }
}

void CMorphoAtkG_Teleport_FireWall::Exit_Step()
{
    switch (m_Step)
    {
    case StateStep::Start:
        break;
    case StateStep::Progress: {
        MRPFSM->OffWeaponRTrigger();
    }
    break;
    case StateStep::End: {
        GamePlayStatic::DestroyGameObject(m_FireWall);
        m_FireWall = nullptr;
    }
        break;
    }
}

void CMorphoAtkG_Teleport_FireWall::Start()
{
    RotateToPlayer(DT * 0.75f);

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkG_Teleport_FireWall::Progress()
{
    MoveFireWall();

    if (m_bFrmEnter)
    {
        RotateToPlayer(DT * 0.75f);
    }

    // spawn Fire Wall
    if (m_bFrmEnter && CHECK_ANIMFRM(GetOwner(), 30))
    {
        m_bFrmEnter = false;

        if (m_FireWallPref != nullptr)
        {
            m_FireWall = m_FireWallPref->Instantiate();

            Vec3 Pos = GetOwner()->Transform()->GetWorldPos();
            Pos.y = 0.f;
            Vec3 Dir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);
            Dir.y = 0.f;
            Dir.Normalize();

            m_FireWall->Transform()->SetWorldPos(Pos);
            m_FireWall->Transform()->Slerp(Dir, 1.f);
            m_FireWall->Animator()->Play(ANIMPREFIX("Main"), false, false, 1.f, 0.f);
            GamePlayStatic::SpawnGameObject(m_FireWall, LAYER_MONSTERATK_TRIGGER);
        }

        CAMERACTRL->Shake(0.3f, 30.f, 30.f);
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkG_Teleport_FireWall::End()
{
    m_AccTime += DT;

    // move fire wall
    MoveFireWall();

    // spawn dropstar
    if (!m_bSpawnDropStar && m_AccTime > 1.f)
    {
        m_bSpawnDropStar = true;
        UINT bOdd = 0;

        for (CGameObject* Fire : m_FireWall->GetChildObject())
        {
            if (!Fire->CapsuleCollider())
                continue;

            if (bOdd == 0)
            {
                Vec3 SpawnPos = Fire->Transform()->GetWorldPos();
                MRPFSM->SpawnDropStar(SpawnPos);
            }

            if (++bOdd > 3)
            {
                bOdd = 0;
            }
        }
    }

    if (m_FireWall && m_FireWall->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}

void CMorphoAtkG_Teleport_FireWall::MoveFireWall()
{
    if (!m_FireWall)
        return;

    Vec3 NewPos = m_FireWall->Transform()->GetWorldPos();
    NewPos += m_FireWall->Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_WallSpeed * DT;
    m_FireWall->Transform()->SetWorldPos(NewPos);
}
