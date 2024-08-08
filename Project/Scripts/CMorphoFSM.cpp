#include "pch.h"
#include "CMorphoFSM.h"
#include "CState.h"
#include "CPlayerMgr.h"

CMorphoFSM::CMorphoFSM()
    : CFSMScript(MORPHOFSM)
    , m_Phase(1)
    , m_ComboLevel(0)
    , m_NearDist(100.f)
    , m_bAttackRepeat(false)
    , m_MapFloorOffset(Vec3())
    , m_MapSize(Vec3())
{
}

CMorphoFSM::~CMorphoFSM()
{
}

#include "CMorpho_Idle.h"
void CMorphoFSM::begin()
{
    // set map size
    float ScaleFactor = Transform()->GetLocalScale().x;
    m_NearDist *= ScaleFactor;
    m_MapFloorOffset *= ScaleFactor;
    m_MapSize *= ScaleFactor;

    // add state
    //AddState(L"MOVE_TOGROUND", new CMorpho_ToGround())
    AddState(L"IDLE", new CMorpho_Idle());

    ChangeState(L"IDLE");
}

void CMorphoFSM::tick()
{
    CFSMScript::tick();
}

void CMorphoFSM::Move()
{
    m_bAttackRepeat = false;
    m_ComboLevel = 0;
    ChangeState(L"MOVE");
}

void CMorphoFSM::Attack()
{
    m_bAttackRepeat = false;
    m_ComboLevel = 0;
    ChangeState(L"ATTACK");
}

void CMorphoFSM::RepeatState(wstring _State)
{
    m_bAttackRepeat = true;

    if (_State != L"")
    {
        ChangeState(_State);
    }
    else
    {
        ChangeState(GetCurState()->GetName());
    }
}

float CMorphoFSM::GetPlayerDist() const
{
    Vec3 Dist = PLAYER->Transform()->GetWorldPos() - GetOwner()->Transform()->GetWorldPos();
    Dist.y = 0.f;
    return Dist.Length();
}

UINT CMorphoFSM::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CMorphoFSM::LoadFromLevelFile(FILE* _File)
{
    return 0;
}