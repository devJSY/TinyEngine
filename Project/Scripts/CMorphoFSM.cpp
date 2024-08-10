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
    for (UINT i = 0; i < (UINT)MorphoStateGroup::END; ++i)
    {
        m_StateGroup[(MorphoStateGroup)i][0] = vector<wstring>();
        m_StateGroup[(MorphoStateGroup)i][1] = vector<wstring>();
    }
}

CMorphoFSM::~CMorphoFSM()
{
}

#include "CMorpho_Idle.h"
#include "CMorphoAtkG_NormalNear_Atk1.h"
#include "CMorphoAtkG_NormalNear_Atk2.h"
#include "CMorphoAtkG_NormalNear_Atk3.h"
#include "CMorphoAtkG_NormalNear_Atk3.h"
//#include "CMorphoAtkG_NormalNear_AtkFlight.h"
#include "CMorphoAtkG_NormalFar_SwordSlash.h"
#include "CMorphoAtkG_Teleport_FireWall.h"
#include "CMorphoAtkG_Teleport_Tornado.h"
#include "CMorphoAtkG_Teleport_TrackingSoul.h"
#include "CMorphoAtkA_ShockWave.h"
void CMorphoFSM::begin()
{
    // set map size
    float ScaleFactor = Transform()->GetLocalScale().x;
    m_NearDist *= ScaleFactor;
    m_MapFloorOffset *= ScaleFactor;
    m_MapSize *= ScaleFactor;

    // shockwave
    m_vecShockWave.push_back(GetOwner()->GetChildObject(L"ShockWaveL"));
    m_vecShockWave.push_back(GetOwner()->GetChildObject(L"ShockWaveR"));

    // add state
    AddGroupPublicState(MorphoStateGroup::Idle, L"IDLE", new CMorpho_Idle);
    AddGroupPublicState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATK1", new CMorphoAtkG_NormalNear_Atk1);
    //AddGroupPublicState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATKFLIGHT", new CMorphoAtkG_NormalNear_AtkFlight);
    AddGroupPublicState(MorphoStateGroup::AtkGroundNormalFar, L"ATKG_NORMALFAR_SWORDSLASH", new CMorphoAtkG_NormalFar_SwordSlash);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport, L"ATKG_TELEPORT_FIREWALL", new CMorphoAtkG_Teleport_FireWall);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport, L"ATKG_TELEPORT_TORNADO", new CMorphoAtkG_Teleport_Tornado);
    AddGroupPublicState(MorphoStateGroup::AtkGroundTeleport, L"ATKG_TELEPORT_TRACKINGSOUL", new CMorphoAtkG_Teleport_TrackingSoul);
    AddGroupPublicState(MorphoStateGroup::AtkAir, L"ATKA_SHOCKWAVE", new CMorphoAtkA_ShockWave);

    AddGroupPrivateState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATK2", new CMorphoAtkG_NormalNear_Atk2);
    AddGroupPrivateState(MorphoStateGroup::AtkGroundNormalNear, L"ATKG_NORMALNEAR_ATK3", new CMorphoAtkG_NormalNear_Atk3);

    ChangeStateGroup(MorphoStateGroup::Idle);
}

void CMorphoFSM::tick()
{
    CFSMScript::tick();

    if (KEY_TAP(KEY::ENTER))
    {
        ChangeStateGroup(MorphoStateGroup::AtkAir, L"ATKA_SHOCKWAVE");
    }
}

void CMorphoFSM::Move()
{
    m_bAttackRepeat = false;
    m_ComboLevel = 0;

    //@TODO 구현후 복구
    ChangeStateGroup(MorphoStateGroup::Idle);
    return;

    float Rand = GetRandomfloat(1.f, 100.f);

    if (Rand <= 70.f)
    {
        ChangeStateGroup(MorphoStateGroup::MoveToGround);
    }
    else
    {
        ChangeStateGroup(MorphoStateGroup::MoveToAir);
    }
}


void CMorphoFSM::Attack()
{
    m_bAttackRepeat = false;
    m_ComboLevel = 0;

    // Move (To Ground)
    if (m_CurStateGroup == MorphoStateGroup::MoveToGround)
    {
        float Rand = GetRandomfloat(1.f, 100.f);

        // wait
        if (Rand <= 10.f)
        {
            ChangeStateGroup(MorphoStateGroup::AtkGroundWait);
        }

        // attack
        else
        {
            // attack normal
            if (Rand <= 55.f)
            {
                if (IsNearPlayer())
                {
                    ChangeStateGroup(MorphoStateGroup::AtkGroundNormalNear);
                }
                else
                {
                    ChangeStateGroup(MorphoStateGroup::AtkGroundNormalFar);
                }
            }

            // attack teleport
            else
            {
                ChangeStateGroup(MorphoStateGroup::AtkGroundTeleport);
            }
        }
    }

    // Move (To Air)
    else if (m_CurStateGroup == MorphoStateGroup::MoveToAir)
    {
        ChangeStateGroup(MorphoStateGroup::AtkAir);
    }
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

void CMorphoFSM::ChangeStateGroup(MorphoStateGroup _Group, const wstring& _State)
{
    if (_State.empty())
    {
        ChangeStateGroup_Random(_Group);
    }
    else
    {
        ChangeStateGroup_Set(_Group, _State);
    }
}


void CMorphoFSM::ChangeStateGroup_Random(MorphoStateGroup _Group)
{
    if (m_CurStateGroup == _Group || m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    int Random = GetRandomInt(0, (int)m_StateGroup[_Group][0].size() - 1);
    wstring RandState = m_StateGroup[_Group][0][Random];

    m_CurStateGroup = _Group;
    ChangeState(RandState);
}

void CMorphoFSM::ChangeStateGroup_Set(MorphoStateGroup _Group, const wstring& _State)
{
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    vector<wstring>::iterator iter1 = find(m_StateGroup[_Group][0].begin(), m_StateGroup[_Group][0].end(), _State);
    vector<wstring>::iterator iter2 = find(m_StateGroup[_Group][1].begin(), m_StateGroup[_Group][1].end(), _State);
    if (iter1 == m_StateGroup[_Group][0].end() && iter2 == m_StateGroup[_Group][1].end())
        return;

    m_CurStateGroup = _Group;
    ChangeState(_State);
}

void CMorphoFSM::AddGroupPublicState(MorphoStateGroup _Group, const wstring& _StateName, CState* _State)
{
    // 이미 존재하는 Group에만 추가
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    CFSMScript::AddState(_StateName, _State);
    m_StateGroup[_Group][0].push_back(_StateName);
}

void CMorphoFSM::AddGroupPrivateState(MorphoStateGroup _Group, const wstring& _StateName, CState* _State)
{
    // 이미 존재하는 Group에만 추가
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    CFSMScript::AddState(_StateName, _State);
    m_StateGroup[_Group][1].push_back(_StateName);
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