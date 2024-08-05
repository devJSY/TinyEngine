#include "pch.h"
#include "CElfilisFSM.h"
#include "CElfilisBigFSM.h"
#include "CState.h"
#include "CPlayerMgr.h"

CElfilisFSM::CElfilisFSM()
    : CFSMScript(ELFILISFSM)
    , m_CurStateGroup(ElfilisStateGroup::END)
    , m_Pattern(ElfilisPatternType::NONE)
    , m_Phase(2)
    , m_ComboLevel(0)
    , m_PatternStep(0)
    , m_bAttackRepeat(false)
    , m_GroundAttackCount(0)
    , m_NearDist(5.f)
    , m_AirPosition(Vec3(0.f, 600.f, 1000.f))
    , m_bGroundCollision(false)
    , m_BigElfilis(nullptr)
    , m_BigElfilisFSM(nullptr)
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
    , m_Pattern(ElfilisPatternType::NONE)
    , m_Phase(_Origin.m_Phase)
    , m_ComboLevel(0)
    , m_PatternStep(0)
    , m_bAttackRepeat(false)
    , m_GroundAttackCount(0)
    , m_NearDist(_Origin.m_NearDist)
    , m_AirPosition(_Origin.m_NearDist)
    , m_bGroundCollision(false)
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

// USAGE : 안전하게 State Group을 변경
void CElfilisFSM::ChangeStateGroup(ElfilisStateGroup _Group, const wstring& _State)
{
    if (m_Pattern != ElfilisPatternType::NONE)
    {
        ProcPatternStep();
        return;
    }

    if (_State.empty())
    {
        ChangeStateGroup_Random(_Group);
    }
    else
    {
        ChangeStateGroup_Set(_Group, _State);
    }
}

void CElfilisFSM::ChangeStateGroup_Random(ElfilisStateGroup _Group)
{
    if (m_CurStateGroup == _Group || m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    int Random = GetRandomInt(0, (int)m_StateGroup[_Group][0].size() - 1);
    wstring RandState = m_StateGroup[_Group][0][Random];

    SetStateGroup(_Group);
    ChangeState(RandState);
}

void CElfilisFSM::ChangeStateGroup_Set(ElfilisStateGroup _Group, const wstring& _State)
{
    if (m_StateGroup.find(_Group) == m_StateGroup.cend())
        return;

    vector<wstring>::iterator iter1 = find(m_StateGroup[_Group][0].begin(), m_StateGroup[_Group][0].end(), _State);
    vector<wstring>::iterator iter2 = find(m_StateGroup[_Group][1].begin(), m_StateGroup[_Group][1].end(), _State);
    if (iter1 == m_StateGroup[_Group][0].end() && iter2 == m_StateGroup[_Group][1].end())
        return;

    SetStateGroup(_Group);
    ChangeState(_State);
}

void CElfilisFSM::RepeatState(wstring _State)
{
    if (m_Pattern != ElfilisPatternType::NONE)
    {
        ProcPatternStep();
        return;
    }

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
            return ElfilisStateGroup::AirLargeAtk;
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

        if (m_Phase == 1)
        {
            return ElfilisStateGroup::AirSmallAtk1;
        }
        else
        {
            float Rand = GetRandomfloat(1, 100);

            if (Rand <= 50.f)
            {
                return ElfilisStateGroup::AirSmallAtk1;
            }
            else
            {
                return ElfilisStateGroup::AirSmallAtk2;
            }
        }
    }
    break;
    case ElfilisStateGroup::AirSmallAtk1: {
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
    case ElfilisStateGroup::AirSmallAtk2:
    case ElfilisStateGroup::AirLargeAtk: {
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
#include "CElfilisG_ToAirTeleport.h"

#include "CElfilisA_Idle.h"
#include "CElfilisA_MoveL.h"
#include "CElfilisA_MoveR.h"
#include "CElfilisA_RayArrowUp.h"
// #include "CElfilisA_RayArrowDown.h"
#include "CElfilisA_StabCombo.h"
#include "CElfilisA_SlashCombo.h"
#include "CElfilisA_DimensionLaser.h"
#include "CElfilisA_DrawLaser.h"
#include "CElfilisA_Stab.h"
#include "CElfilisA_Teleport.h"
#include "CElfilisA_TeleportCombo.h"
void CElfilisFSM::begin()
{
    // set map size
    float ScaleFactor = Transform()->GetLocalScale().x;
    m_NearDist *= ScaleFactor;
    m_MapFloorOffset *= ScaleFactor;
    m_MapSizeRadius *= ScaleFactor;

    // add state
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
    AddGroupPublicState(ElfilisStateGroup::AirSmallAtk1, L"AIR_ATKS_RAYARROW_UP", new CElfilisA_RayArrowUp);
    // AddGroupPublicState(ElfilisStateGroup::AirSmallAtk, L"AIR_ATKS_RAYARROW_DOWN", new CElfilisA_RayArrowDown);
    AddGroupPublicState(ElfilisStateGroup::AirSmallAtk1, L"AIR_ATKS_STABCOMBO", new CElfilisA_StabCombo);
    AddGroupPublicState(ElfilisStateGroup::AirSmallAtk1, L"AIR_ATKS_SLASHCOMBO", new CElfilisA_SlashCombo);
    AddGroupPublicState(ElfilisStateGroup::AirSmallAtk2, L"AIR_ATKS_DIMENSIONLASER", new CElfilisA_DimensionLaser);
    AddGroupPublicState(ElfilisStateGroup::AirLargeAtk, L"AIR_ATKL_DRAWLASER", new CElfilisA_DrawLaser);
    AddGroupPublicState(ElfilisStateGroup::AirToGround, L"AIR_TOGROUND_TELEPORT", new CElfilisA_Teleport);
    AddGroupPublicState(ElfilisStateGroup::AirToGround, L"AIR_TOGROUND_TELEPORTCOMBO", new CElfilisA_TeleportCombo);

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
    AddGroupPrivateState(ElfilisStateGroup::GroundToAir, L"GROUND_TOAIR_TELEPORT", new CElfilisG_ToAirTeleport);
    AddGroupPrivateState(ElfilisStateGroup::AirToGround, L"AIR_TOGROUND_STAB", new CElfilisA_Stab);

    ChangeState(L"GROUND_IDLE");

    // find Big Elfilis
    wstring strName = GetOwner()->GetName() + L"Big";
    CGameObject* pBossBig = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(strName, LAYER_MONSTER);
    CElfilisBigFSM* pBossBigFSM = nullptr;

    if (pBossBig)
    {
        pBossBigFSM = pBossBig->GetScript<CElfilisBigFSM>();
    }

    if (pBossBig && pBossBigFSM)
    {
        m_BigElfilis = pBossBig;
        m_BigElfilisFSM = pBossBigFSM;
    }
    else
    {
        MessageBox(nullptr, L"Big Elfilis를 찾을 수 없습니다", L"Big Elfilis 등록 실패", MB_OK);
    }
}

void CElfilisFSM::tick()
{
    CFSMScript::tick();
}

void CElfilisFSM::OnCollisionEnter(CCollider* _OtherCollider)
{
    // static vector<wstring> vecCollision{L"World Static", L"World Dynamic"};
    // RaycastHit Hit = CPhysicsMgr::GetInst()->RayCast(GetOwner()->Transform()->GetWorldPos(), Vec3(0.f, -1.f, 0.f), 100.f, vecCollision);
    int LayerIdx = _OtherCollider->GetOwner()->GetLayerIdx();

    if (m_CurStateGroup == ElfilisStateGroup::AirToGround)
    {
        if (LayerIdx == LAYER_STATIC && _OtherCollider->GetOwner()->GetName() == L"Floor")
        {
            m_bGroundCollision = true;
        }
        else if (LayerIdx == LAYER_PLAYER)
        {
            m_bGroundCollision = true;
        }
    }
}

void CElfilisFSM::SetPattern(ElfilisPatternType _Pattern)
{
    m_PatternStep = 0;
    m_Pattern = _Pattern;
}

void CElfilisFSM::ProcPatternStep()
{
    if (m_Pattern == ElfilisPatternType::NONE)
        return;

    bool bFinish = false;

    switch (m_Pattern)
    {
    case ElfilisPatternType::BigCombo: {
        if (m_PatternStep == 0)
        {
            ChangeStateGroup_Random(ElfilisStateGroup::GroundIdle);
        }
        else if (m_PatternStep == 1) // 진입 : 외부호출
        {
            ChangeStateGroup_Set(ElfilisStateGroup::GroundToAir, L"GROUND_TOAIR");
        }
        else if (m_PatternStep == 2)
        {
            ChangeStateGroup_Set(ElfilisStateGroup::AirIdle, L"AIR_IDLE");
        }
        else if (m_PatternStep == 3) // 진입 : 외부호출
        {
            ChangeStateGroup_Random(ElfilisStateGroup::AirSmallAtk1);
        }
        else if (m_PatternStep == 4)
        {
            ChangeStateGroup_Random(ElfilisStateGroup::AirSmallAtk1);
        }
        else if (m_PatternStep == 5)
        {
            ChangeStateGroup_Random(ElfilisStateGroup::AirIdle);
        }
        else if (m_PatternStep == 6) // 진입 : 외부호출
        {
            ChangeStateGroup_Set(ElfilisStateGroup::AirToGround, L"AIR_TOGROUND_STAB");
            bFinish = true;
        }
    }
    break;
    case ElfilisPatternType::StabCombo: {
        if (m_PatternStep == 0)
        {
            ChangeStateGroup_Set(ElfilisStateGroup::AirToGround, L"AIR_TOGROUND_STAB");
        }
        else if (m_PatternStep == 1)
        {
            ChangeStateGroup_Set(ElfilisStateGroup::GroundToAir, L"GROUND_TOAIR_TELEPORT");
        }
        else if (m_PatternStep == 2)
        {
            ChangeStateGroup_Set(ElfilisStateGroup::AirToGround, L"AIR_TOGROUND_STAB");
            bFinish = true;
        }
    }
    break;
    }

    if (bFinish)
    {
        SetPattern(ElfilisPatternType::NONE);
    }
    else
    {
        m_PatternStep++;
    }
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

void CElfilisFSM::SetStateGroup(ElfilisStateGroup _Group)
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
    case ElfilisStateGroup::AirSmallAtk1:
    case ElfilisStateGroup::AirSmallAtk2:
    case ElfilisStateGroup::AirLargeAtk:
        ClearComboLevel();
        m_bAttackRepeat = false;
        break;
    case ElfilisStateGroup::AirToGround: {
        ClearComboLevel();
        m_bAttackRepeat = false;
        m_GroundAttackCount = 0;
        m_bGroundCollision = false;
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