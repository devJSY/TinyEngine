#include "pch.h"
#include "CMorphoAtkG_Teleport_FireWall.h"
#include "CMorphoFSM.h"

CMorphoAtkG_Teleport_FireWall::CMorphoAtkG_Teleport_FireWall()
    : m_FireWall(nullptr)
    , m_bWallSpawn(false)
    , m_WallSpeed(150.f)
{
    Ptr<CPrefab> Pref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\MorphoFireWall.pref", L"prefab\\MorphoFireWall.pref");

    if (Pref != nullptr)
    {
        m_FireWall = Pref->Instantiate();
    }
}

CMorphoAtkG_Teleport_FireWall::~CMorphoAtkG_Teleport_FireWall()
{
    if (m_FireWall && (!m_bWallSpawn || m_FireWall->GetLayerIdx() == -1))
    {
        delete m_FireWall;
    }
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
    }
    break;
    case StateStep::End: {
        GetOwner()->Animator()->Play(ANIMPREFIX("GigaMoonShotCombWait"), false, false, 1.5f);
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
    case StateStep::Progress:
        break;
    case StateStep::End: {
        m_bWallSpawn = false;
        GamePlayStatic::DetachObject(m_FireWall);
    }
    break;
    }
}

void CMorphoAtkG_Teleport_FireWall::Start()
{
    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::Progress);
    }
}

void CMorphoAtkG_Teleport_FireWall::Progress()
{
    MoveFireWall();

    // spawn Fire Wall
    if (!m_bWallSpawn && CHECK_ANIMFRM(GetOwner(), 30))
    {
        m_bWallSpawn = true;

        if (m_FireWall)
        {
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
    }

    if (GetOwner()->Animator()->IsFinish())
    {
        ChangeStep(StateStep::End);
    }
}

void CMorphoAtkG_Teleport_FireWall::End()
{
    MoveFireWall();

    if (m_FireWall && m_FireWall->Animator()->IsFinish())
    {
        MRPFSM->Move();
    }
}

void CMorphoAtkG_Teleport_FireWall::MoveFireWall()
{
    if (!m_bWallSpawn)
        return;

    Vec3 NewPos = m_FireWall->Transform()->GetWorldPos();
    NewPos += m_FireWall->Transform()->GetWorldDir(DIR_TYPE::FRONT) * m_WallSpeed * DT;
    m_FireWall->Transform()->SetWorldPos(NewPos);
}
