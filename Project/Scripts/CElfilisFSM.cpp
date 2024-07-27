#include "pch.h"
#include "CElfilisFSM.h"
#include "CState.h"

CElfilisFSM::CElfilisFSM()
    : CFSMScript(ELFILISFSM)
    , m_CurStateGroup(ElfilisStateGroup::END)
    , m_GroundAttackCount(0)
    , m_bAttackRepeat(false)
{
    // 에피리스FSM은 GroundMove, GroundAtk, GroundToAir, / AirMove, AirSmallATK, AirBigATK, AirToGround State를 가짐
    // 각 State는 유사한 State를 묶은 Group State
    // 마치 FSM처럼 State를 담고 있는 State
    // ex, GourndMove는 {순간이동, 포탈찌르기, 백스텝} 등의 State를 들고 있음
    // ex, GroundMove
    //? 아닌데? Hierarchy State는 일련의 State 흐름을 묶은거아닌가? (ex GroundMove -> GroundAttack 반복구조)
    //? 아닌데? 즉 GroundMove만으로는 Hierachy State가 될 수 없고, Ground 상태를 통틀어야지 Hierachy State가 될 수 있음

    for (UINT i = 0; i < (UINT)ElfilisStateGroup::END; ++i)
    {
        m_StateGroup[(ElfilisStateGroup)i][0] = vector<wstring>();
        m_StateGroup[(ElfilisStateGroup)i][1] = vector<wstring>();
    }
}

CElfilisFSM::CElfilisFSM(const CElfilisFSM& _Origin)
    : CFSMScript(_Origin)
    , m_CurStateGroup(ElfilisStateGroup::END)
    , m_GroundAttackCount(0)
    , m_bAttackRepeat(false)
{
    for (auto it : m_StateGroup)
    {
        for (auto state : it.second[0])
        {
            m_StateGroup[it.first][0].push_back(state);
        }
        for (auto state : it.second[1])
        {
            m_StateGroup[it.first][1].push_back(state);
        }
    }
}

CElfilisFSM::~CElfilisFSM()
{
}

void CElfilisFSM::ChangeStateGroup_RandState(ElfilisStateGroup _Group)
{
    if (m_CurStateGroup == _Group || m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    int Random = GetRandomInt(0, m_StateGroup[_Group][0].size() - 1);
    wstring RandState = m_StateGroup[_Group][0][Random];

    ChangStateGroup(_Group);
    ChangeState(RandState);
}

void CElfilisFSM::ChangeStateGroup_SetState(ElfilisStateGroup _Group, const wstring& _State)
{
    if (m_CurStateGroup == _Group || m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    ChangStateGroup(_Group);
    ChangeState(_State);
}

void CElfilisFSM::RepeatState()
{
    m_bAttackRepeat = true;
    ChangeState(GetCurState()->GetName());
}

ElfilisStateGroup CElfilisFSM::FindNextStateGroup()
{
    // Usage : GroundMove -> GroundAttack -> ? 에서 "?"가 먼지 찾으려고
    // 즉, Attack 반복인지, Air인지, Move로 돌아가는지

    switch (m_CurStateGroup)
    {
    case ElfilisStateGroup::GroundMove:
    case ElfilisStateGroup::GroundMoveAtk: {
        //@TODO 전이확인시 복구
        return ElfilisStateGroup::GroundIdle;

        float Rand = GetRandomfloat(1, 100);
        if (Rand <= 90.f)
        {
            return ElfilisStateGroup::GroundAtk;
        }
        else
        {
            return ElfilisStateGroup::GroundAtk;
        }
    }
    break;
    case ElfilisStateGroup::GroundAtk: {
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
                return ElfilisStateGroup::GroundAtk;
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
    case ElfilisStateGroup::GroundToAir:
        break;
    case ElfilisStateGroup::AirIdle:
    case ElfilisStateGroup::AirMove:
    case ElfilisStateGroup::AirSmallAtk:
    case ElfilisStateGroup::AirBigAtk:
    case ElfilisStateGroup::AirToGround:
    case ElfilisStateGroup::END:
        break;
    }

    return ElfilisStateGroup::END;
}

#include "CElfilisG_Idle.h"
#include "CElfilisG_BackStep.h"
#include "CElfilisG_NormalAtkL.h"
void CElfilisFSM::begin()
{
    // State 추가
    AddGroupPublicState(ElfilisStateGroup::GroundIdle, L"GROUND_IDLE", new CElfilisG_Idle);
    AddGroupPublicState(ElfilisStateGroup::GroundMove, L"BACKSTEP", new CElfilisG_BackStep);
    AddGroupPublicState(ElfilisStateGroup::GroundAtk, L"GROUND_ATK_L", new CElfilisG_NormalAtkL);
    // AddGroupPrivateState(ElfilisStateGroup::GrondIdle, L"GROUND_IDLE", new CElfilisGroundIdle);

    //ChangeStateGroup_SetState(ElfilisStateGroup::GrondIdle, L"GROUND_IDLE");
    ChangeState(L"GROUND_IDLE");
}

void CElfilisFSM::tick()
{
    // ex Ground상태일 때
    //? GroundMove Group이 끝나면 -> GroundAttack의 랜덤상태로 던지고싶음
    //? 이걸 Ground 한 세트로 보고,

    CFSMScript::tick();
}

const vector<wstring>& CElfilisFSM::GetCurPublicStates() const
{
    map<ElfilisStateGroup, vector<wstring>[2]>::const_iterator it = m_StateGroup.find(m_CurStateGroup);
    if (it != m_StateGroup.end())
    {
        return it->second[0];
    }
    return vector<wstring>();
}

void CElfilisFSM::ChangStateGroup(ElfilisStateGroup _Group)
{
    // check count
    switch (_Group)
    {
    case ElfilisStateGroup::GroundIdle:
        break;
    case ElfilisStateGroup::GroundMove:
    case ElfilisStateGroup::GroundMoveAtk:
    {
        m_bAttackRepeat = false;
    }
        break;
    case ElfilisStateGroup::GroundAtk: {
        m_GroundAttackCount++;
    }
    break;
    case ElfilisStateGroup::GroundToAir: {
        m_bAttackRepeat = false;
    }
        break;
    case ElfilisStateGroup::AirIdle:
        break;
    case ElfilisStateGroup::AirMove:
        break;
    case ElfilisStateGroup::AirSmallAtk:
        break;
    case ElfilisStateGroup::AirBigAtk:
        break;
    case ElfilisStateGroup::AirToGround:
    {
        m_GroundAttackCount = 0;
        m_bAttackRepeat = false;
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