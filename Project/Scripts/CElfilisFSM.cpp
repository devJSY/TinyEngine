#include "pch.h"
#include "CElfilisFSM.h"
#include "CState.h"
#include "CPlayerMgr.h"

CElfilisFSM::CElfilisFSM()
    : CFSMScript(ELFILISFSM)
    , m_CurStateGroup(ElfilisStateGroup::END)
    , m_ReserveState{ElfilisStateGroup::END, L""}
    , m_Phase(1)
    , m_ComboLevel(0)
    , m_bAttackRepeat(false)
    , m_GroundAttackCount(0)
    , m_NearDist(5.f)
    , m_AirPosition(Vec3(0.f, 600.f, 1000.f))
    , m_MapFloorOffset(Vec3(0.f, 0.f, 5.f))
    , m_MapSizeRadius(25.f)
{
    for (UINT i = 0; i < (UINT)ElfilisStateGroup::END; ++i)
    {
        m_StateGroup[(ElfilisStateGroup)i][0] = vector<wstring>();
        m_StateGroup[(ElfilisStateGroup)i][1] = vector<wstring>();
    }
}

CElfilisFSM::CElfilisFSM(const CElfilisFSM& _Origin)
    : CFSMScript(_Origin)
    , m_CurStateGroup(ElfilisStateGroup::END)
    , m_ReserveState{ElfilisStateGroup::END, L""}
    , m_Phase(_Origin.m_Phase)
    , m_ComboLevel(0)
    , m_bAttackRepeat(false)
    , m_GroundAttackCount(0)
    , m_NearDist(_Origin.m_NearDist)
    , m_AirPosition(_Origin.m_NearDist)
    , m_MapFloorOffset(_Origin.m_MapFloorOffset)
    , m_MapSizeRadius(_Origin.m_MapSizeRadius)
{
    for (auto it : m_StateGroup)
    {
        m_StateGroup[(ElfilisStateGroup)it.first][0] = vector<wstring>();
        m_StateGroup[(ElfilisStateGroup)it.first][1] = vector<wstring>();
    }

    // for (auto it : m_StateGroup)
    //{
    //     for (auto state : it.second[0])
    //     {
    //         m_StateGroup[it.first][0].push_back(state);
    //     }
    //     for (auto state : it.second[1])
    //     {
    //         m_StateGroup[it.first][1].push_back(state);
    //     }
    // }
}

CElfilisFSM::~CElfilisFSM()
{
}

void CElfilisFSM::ChangeStateGroup_RandState(ElfilisStateGroup _Group)
{
    if (m_CurStateGroup == _Group || m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    int Random = GetRandomInt(0, (int)m_StateGroup[_Group][0].size() - 1);
    wstring RandState = m_StateGroup[_Group][0][Random];

    ChangStateGroup(_Group);
    ChangeState(RandState);
}

void CElfilisFSM::ChangeStateGroup_SetState(ElfilisStateGroup _Group, const wstring& _State)
{
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    vector<wstring>::iterator iter1 = find(m_StateGroup[_Group][0].begin(), m_StateGroup[_Group][0].end(), _State);
    vector<wstring>::iterator iter2 = find(m_StateGroup[_Group][1].begin(), m_StateGroup[_Group][1].end(), _State);
    if (iter1 == m_StateGroup[_Group][0].end() && iter2 == m_StateGroup[_Group][1].end())
        return;

    ChangStateGroup(_Group);
    ChangeState(_State);
}

void CElfilisFSM::ReserveState(ElfilisStateGroup _Group, const wstring& _State)
{
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    vector<wstring>::iterator iter1 = find(m_StateGroup[_Group][0].begin(), m_StateGroup[_Group][0].end(), _State);
    vector<wstring>::iterator iter2 = find(m_StateGroup[_Group][1].begin(), m_StateGroup[_Group][1].end(), _State);
    if (iter1 == m_StateGroup[_Group][0].end() && iter2 == m_StateGroup[_Group][1].end())
        return;

    m_ReserveState = {_Group, _State};
}

void CElfilisFSM::RepeatState(wstring _State)
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

// Usage : GroundAttackCount, Repeat 등의 값을 체크해 필요한 다음 Group State를 찾음
ElfilisStateGroup CElfilisFSM::FindNextStateGroup() const
{
    switch (m_CurStateGroup)
    {
    case ElfilisStateGroup::GroundIdle: {
        return ElfilisStateGroup::GroundMove;
    }
    break;
    case ElfilisStateGroup::GroundMove:
    case ElfilisStateGroup::GroundMoveAtk: {
        float Rand = GetRandomfloat(1, 100);

        if (Rand <= 90.f)
        {
            if (GetPlayerDist() > 100.f)
            {
                return ElfilisStateGroup::GroundAtkFar;
            }
            else
            {
                return ElfilisStateGroup::GroundAtkNear;
            }
        }
        else
        {
            return ElfilisStateGroup::GroundToAir;
        }
    }
    break;

    case ElfilisStateGroup::GroundAtkNear:
    case ElfilisStateGroup::GroundAtkFar: {
        // case : 이미 같은 공격을 2번 반복한 이후
        if (m_bAttackRepeat)
        {
            if (m_GroundAttackCount >= 2)
            {
                return ElfilisStateGroup::GroundToAir;
            }
            else
            {
                float Rand = GetRandomfloat(1, 100);

                if (Rand <= 40.f)
                {
                    return ElfilisStateGroup::GroundMove;
                }
                else if (Rand <= 80.f)
                {
                    return ElfilisStateGroup::GroundMoveAtk;
                }
                else
                {
                    return ElfilisStateGroup::GroundToAir;
                }
            }
        }

        // case : 최초 공격의 경우
        else
        {
            float Rand = GetRandomfloat(1, 100);

            if (Rand <= 50.f)
            {
                // 같은 Group State가 retrun되는 경우, 반복의 의미 (함수 호출한 쪽에서 직접 값 확인 후 처리)
                return m_CurStateGroup;
            }
            else if (Rand <= 65.f)
            {
                return ElfilisStateGroup::GroundMove;
            }
            else if (Rand <= 80.f)
            {
                return ElfilisStateGroup::GroundMoveAtk;
            }
            else
            {
                return ElfilisStateGroup::GroundToAir;
            }
        }
    }
    break;
    case ElfilisStateGroup::GroundToAir: {
        //@TODO 전이 확인 후 복구
        return ElfilisStateGroup::AirIdle;

        float Rand = GetRandomfloat(1, 100);

        if (Rand <= 50.f)
        {
            return ElfilisStateGroup::AirMove;
        }
        else if (Rand <= 75.f)
        {
            return ElfilisStateGroup::AirBigAtk;
        }
        else
        {
            return ElfilisStateGroup::AirToGround;
        }
    }
    break;
    case ElfilisStateGroup::AirIdle: {
        //@TODO 전이 확인 후 다시생각
        return ElfilisStateGroup::AirIdle;
    }
    break;
    case ElfilisStateGroup::AirMove: {
        //@TODO 전이 확인 후 복구
        return ElfilisStateGroup::AirIdle;
        return ElfilisStateGroup::AirSmallAtk;
    }
    break;
    case ElfilisStateGroup::AirSmallAtk: {
        // case : 이미 같은 공격을 2번 반복한 이후
        if (m_bAttackRepeat)
        {
            //@TODO 전이 확인 후 복구
            return ElfilisStateGroup::AirIdle;
            return ElfilisStateGroup::AirToGround;
        }

        // case : 최초 공격의 경우
        else
        {
            float Rand = GetRandomfloat(1, 100);

            if (Rand <= 50.f)
            {
                // 같은 Group State가 retrun되는 경우, 반복의 의미 (함수 호출한 쪽에서 직접 값 확인 후 처리)
                return m_CurStateGroup;
            }
            else
            {
                //@TODO 전이 확인 후 복구
                return ElfilisStateGroup::AirIdle;
                return ElfilisStateGroup::AirToGround;
            }
        }
    }
    break;
    case ElfilisStateGroup::AirBigAtk: {
        //@TODO 전이 확인 후 복구
        return ElfilisStateGroup::AirIdle;

        return ElfilisStateGroup::AirToGround;
    }
    break;
    case ElfilisStateGroup::AirToGround: {
        if (GetPlayerDist() > 100.f)
        {
            return ElfilisStateGroup::GroundAtkFar;
        }
        else
        {
            return ElfilisStateGroup::GroundAtkNear;
        }
    }
    break;
    }

    return ElfilisStateGroup::END;
}

#include "CElfilisG_Idle.h"
#include "CElfilisG_BackStep.h"
#include "CElfilisG_Teleport.h"
#include "CElfilisG_NormalAtk.h"
#include "CElfilisG_NormalAtkL.h"
#include "CElfilisG_NormalAtkR.h"
#include "CElfilisG_NormalAtkFinishL.h"
#include "CElfilisG_RayArrow.h"
#include "CElfilisG_SwordWaveRL.h"
#include "CElfilisG_SwordWaveLR.h"
#include "CElfilisG_SwordWaveFinishRL.h"
#include "CElfilisG_SwordWaveStorm.h"
// #include "CElfilisG_DimensionSpike.h"
#include "CElfilisG_NormalAtkTeleport.h"
#include "CElfilisG_NormalAtkTeleportL.h"
#include "CElfilisG_NormalAtkTeleportR.h"
#include "CElfilisG_NormalAtkTeleportFinishL.h"
#include "CElfilisG_GroundToAir.h"

#include "CElfilisA_Idle.h"
#include "CElfilisA_MoveL.h"
#include "CElfilisA_MoveR.h"
#include "CElfilisA_RayArrowUp.h"
//#include "CElfilisA_RayArrowDown.h"
void CElfilisFSM::begin()
{
    float ScaleFactor = Transform()->GetLocalScale().x;
    m_NearDist *= ScaleFactor;
    m_MapFloorOffset *= ScaleFactor;
    m_MapSizeRadius *= ScaleFactor;

    // State 추가
    AddGroupPublicState(ElfilisStateGroup::GroundIdle, L"GROUND_IDLE", new CElfilisG_Idle);
    AddGroupPublicState(ElfilisStateGroup::GroundMove, L"GROUND_MOVE_BACKSTEP", new CElfilisG_BackStep);
    AddGroupPublicState(ElfilisStateGroup::GroundMove, L"GROUND_MOVE_TELEPORT", new CElfilisG_Teleport);
    AddGroupPublicState(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMAL", new CElfilisG_NormalAtk);
    AddGroupPublicState(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_RAYARROW", new CElfilisG_RayArrow);
    AddGroupPublicState(ElfilisStateGroup::GroundMoveAtk, L"GROUND_MOVEATK_NORMALTELEPORT", new CElfilisG_NormalAtkTeleport);
    AddGroupPublicState(ElfilisStateGroup::GroundToAir, L"GROUND_TOAIR", new CElfilisG_GroundToAir);
    AddGroupPublicState(ElfilisStateGroup::AirIdle, L"AIR_IDLE", new CElfilisA_Idle);
    AddGroupPublicState(ElfilisStateGroup::AirMove, L"AIR_MOVE_L", new CElfilisA_MoveL);
    AddGroupPublicState(ElfilisStateGroup::AirMove, L"AIR_MOVE_R", new CElfilisA_MoveR);
    AddGroupPublicState(ElfilisStateGroup::AirSmallAtk, L"AIR_ATKS_RAYARROW_UP", new CElfilisA_RayArrowUp);
    //AddGroupPublicState(ElfilisStateGroup::AirSmallAtk, L"AIR_ATKS_RAYARROW_DOWN", new CElfilisA_RayArrowDown);

    AddGroupPrivateState(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMAL_L", new CElfilisG_NormalAtkL);
    AddGroupPrivateState(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMAL_R", new CElfilisG_NormalAtkR);
    AddGroupPrivateState(ElfilisStateGroup::GroundAtkNear, L"GROUND_ATK_NORMAL_FINISHL", new CElfilisG_NormalAtkFinishL);
    AddGroupPrivateState(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_SWORDWAVE_RL", new CElfilisG_SwordWaveRL);
    AddGroupPrivateState(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_SWORDWAVE_LR", new CElfilisG_SwordWaveLR);
    AddGroupPrivateState(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_SWORDWAVE_FINISHLR", new CElfilisG_SwordWaveFinishRL);
    AddGroupPrivateState(ElfilisStateGroup::GroundAtkFar, L"GROUND_ATK_SWORDWAVE_STORM", new CElfilisG_SwordWaveStorm);
    // AddGroupPrivateState(ElfilisStateGroup::GroundAtk, L"GROUND_ATK_DIMENSIONSPIKE", new CElfilisG_DimensionSpike);
    AddGroupPrivateState(ElfilisStateGroup::GroundMoveAtk, L"GROUND_MOVEATK_NORMALTELEPORT_L", new CElfilisG_NormalAtkTeleportL);
    AddGroupPrivateState(ElfilisStateGroup::GroundMoveAtk, L"GROUND_MOVEATK_NORMALTELEPORT_R", new CElfilisG_NormalAtkTeleportR);
    AddGroupPrivateState(ElfilisStateGroup::GroundMoveAtk, L"GROUND_MOVEATK_NORMALTELEPORT_FINISHL", new CElfilisG_NormalAtkTeleportFinishL);

    ChangeState(L"GROUND_IDLE");
}

void CElfilisFSM::tick()
{
    CFSMScript::tick();
}

const vector<wstring>& CElfilisFSM::GetCurPublicStates() const
{
    map<ElfilisStateGroup, vector<wstring>[2]>::const_iterator it = m_StateGroup.find(m_CurStateGroup);
    if (it != m_StateGroup.end())
    {
        return it->second[0];
    }
    else
    {
        MessageBox(nullptr, L"현재 State Group을 찾을 수 없습니다!", L"Failed to CElfilisFSM::GetCurPublicStates()", MB_OK);
        static vector<wstring> ret = vector<wstring>();
        return ret;
    }
}

float CElfilisFSM::GetPlayerDist() const
{
    Vec3 Dist = PLAYER->Transform()->GetWorldPos() - BOSS->Transform()->GetWorldPos();
    Dist.y = 0.f;
    return Dist.Length();
}

void CElfilisFSM::ChangStateGroup(ElfilisStateGroup _Group)
{
    if (m_CurStateGroup == _Group)
        return;

    // check count
    switch (_Group)
    {
    case ElfilisStateGroup::GroundIdle: {
        ClearComboLevel();
        m_bAttackRepeat = false;
        m_GroundAttackCount = 0;
    }
    break;
    case ElfilisStateGroup::GroundMove:
    case ElfilisStateGroup::GroundMoveAtk: {
        ClearComboLevel();
        m_bAttackRepeat = false;
    }
    break;
    case ElfilisStateGroup::GroundAtkNear:
    case ElfilisStateGroup::GroundAtkFar: {
        ClearComboLevel();
        m_bAttackRepeat = false;
        m_GroundAttackCount++;
    }
    break;
    case ElfilisStateGroup::GroundToAir: {
        ClearComboLevel();
        m_bAttackRepeat = false;
        m_GroundAttackCount = 0;
    }
    break;
    case ElfilisStateGroup::AirIdle:
    case ElfilisStateGroup::AirMove:
    case ElfilisStateGroup::AirSmallAtk:
    case ElfilisStateGroup::AirBigAtk:
        ClearComboLevel();
        m_bAttackRepeat = false;
        break;
    case ElfilisStateGroup::AirToGround: {
        ClearComboLevel();
        m_bAttackRepeat = false;
        m_GroundAttackCount = 0;
    }
    break;
    }

    m_CurStateGroup = _Group;
}

void CElfilisFSM::AddGroupPublicState(ElfilisStateGroup _Group, const wstring& _StateName, CState* _State)
{
    // 이미 존재하는 Group에만 추가
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    CFSMScript::AddState(_StateName, _State);
    m_StateGroup[_Group][0].push_back(_StateName);
}

void CElfilisFSM::AddGroupPrivateState(ElfilisStateGroup _Group, const wstring& _StateName, CState* _State)
{
    // 이미 존재하는 Group에만 추가
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    CFSMScript::AddState(_StateName, _State);
    m_StateGroup[_Group][1].push_back(_StateName);
}

UINT CElfilisFSM::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    MemoryByte += CFSMScript::SaveToLevelFile(_File);

    return MemoryByte;
}

UINT CElfilisFSM::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    MemoryByte += CFSMScript::SaveToLevelFile(_File);

    return MemoryByte;
}