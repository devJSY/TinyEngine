#include "pch.h"
#include "CKirbyFSM.h"
#include "CKirbyAbility.h"
#include "CKirbyObject.h"

#include "CKirbyAbility_Normal.h"

CKirbyFSM::CKirbyFSM()
    : CFSMScript(KIRBYFSM)
    , m_CurAbility(AbilityCopyType::NORMAL)
    , m_CurObject(ObjectCopyType::NONE)
    , m_arrAbility{}
    , m_arrObject{}
    , m_ChargeAccTime(0.f)
    , m_HoveringAccTime(0.f)
    , m_HoveringLimitTime(5.f)
    , m_LastJump(JumpType::HIGH)
    , m_bStuffed(false)
    , m_bHovering(false)
{
    // @TODO Copy Type마다 추가
    m_arrAbility[(UINT)AbilityCopyType::NORMAL] = new CKirbyAbility_Normal();
}

CKirbyFSM::CKirbyFSM(const CKirbyFSM& _Origin)
    : CFSMScript(_Origin)
    , m_CurAbility(_Origin.m_CurAbility)
    , m_CurObject(_Origin.m_CurObject)
    , m_arrAbility{}
    , m_arrObject{}
    , m_ChargeAccTime(0.f)
    , m_HoveringAccTime(0.f)
    , m_HoveringLimitTime(5.f)
    , m_LastJump(JumpType::HIGH)
    , m_bStuffed(false)
    , m_bHovering(false)
{
    // Ability Copy 복사
    for (UINT i = 0; i < (UINT)AbilityCopyType::END; ++i)
    {
        if (nullptr == _Origin.m_arrAbility[i])
            continue;

        CKirbyAbility* pAbil = _Origin.m_arrAbility[i]->Clone();
        assert(pAbil);
        m_arrAbility[i] = pAbil;
    }

    // Object Copy 복사
    for (UINT i = 0; i < (UINT)ObjectCopyType::END; ++i)
    {
        if (nullptr == _Origin.m_arrObject[i])
            continue;

        CKirbyObject* pObjCopy = _Origin.m_arrObject[i]->Clone();
        assert(pObjCopy);
        m_arrObject[i] = pObjCopy;
    }
}

CKirbyFSM::~CKirbyFSM()
{
    for (UINT i = 0; i < (UINT)AbilityCopyType::END; ++i)
    {
        if (m_arrAbility[i] != nullptr)
        {
            delete m_arrAbility[i];
            m_arrAbility[i] = nullptr;
        }
    }

    for (UINT i = 0; i < (UINT)ObjectCopyType::END; ++i)
    {
        if (m_arrObject[i] != nullptr)
        {
            delete m_arrObject[i];
            m_arrObject[i] = nullptr;
        }
    }
}

#include "CKirbyIdle.h"
#include "CKirbyRun.h"
#include "CKirbyRunStart.h"
#include "CKirbyJump.h"
#include "CKirbyJumpStart.h"
#include "CKirbyJumpFall.h"
#include "CKirbyLanding.h"
#include "CKirbyLandingEnd.h"
#include "CKirbyHovering.h"
#include "CKirbyHoveringStart.h"
#include "CKirbyHoveringFall.h"
#include "CKirbyHoveringLimit.h"
#include "CKirbyHoveringFallLimit.h"
#include "CKirbyHoveringLanding.h"
#include "CKirbyHoveringSpit.h"
#include "CKirbyAttack.h"
#include "CKirbyAttackCharge1.h"
#include "CKirbyAttackCharge1Start.h"
#include "CKirbyAttackCharge1End.h"
#include "CKirbyAttackCharge1Run.h"
#include "CKirbyAttackCharge2.h"
#include "CKirbyAttackCharge2Start.h"
#include "CKirbyAttackCharge2Run.h"
#include "CKirbyStuffed.h"
#include "CKirbyStuffedIdle.h"
#include "CKirbyStuffedRun.h"
#include "CKirbyStuffedJump.h"
#include "CKirbyStuffedJumpFall.h"
#include "CKirbyStuffedLanding.h"

void CKirbyFSM::begin()
{
    // State 추가
    AddState(L"IDLE", new CKirbyIdle);
    AddState(L"RUN", new CKirbyRun);
    AddState(L"RUN_START", new CKirbyRunStart);
    AddState(L"JUMP", new CKirbyJump);
    AddState(L"JUMP_START", new CKirbyJumpStart);
    AddState(L"JUMP_FALL", new CKirbyJumpFall);
    AddState(L"LANDING", new CKirbyLanding);
    AddState(L"LANDING_END", new CKirbyLandingEnd);
    AddState(L"HOVERING", new CKirbyHovering);
    AddState(L"HOVERING_START", new CKirbyHoveringStart);
    AddState(L"HOVERING_FALL", new CKirbyHoveringFall);
    AddState(L"HOVERING_LIMIT", new CKirbyHoveringLimit);
    AddState(L"HOVERING_FALL_LIMIT", new CKirbyHoveringFallLimit);
    AddState(L"HOVERING_LANDING", new CKirbyHoveringLanding);
    AddState(L"HOVERING_SPIT", new CKirbyHoveringSpit);
    AddState(L"ATTACK", new CKirbyAttack);
    AddState(L"ATTACK_CHARGE1", new CKirbyAttackCharge1);
    AddState(L"ATTACK_CHARGE1_START", new CKirbyAttackCharge1Start);
    AddState(L"ATTACK_CHARGE1_END", new CKirbyAttackCharge1End);
    AddState(L"ATTACK_CHARGE1_RUN", new CKirbyAttackCharge1Run);
    AddState(L"ATTACK_CHARGE2", new CKirbyAttackCharge2);
    AddState(L"ATTACK_CHARGE2_START", new CKirbyAttackCharge2Start);
    AddState(L"ATTACK_CHARGE2_RUN", new CKirbyAttackCharge2Run);
    AddState(L"STUFFED", new CKirbyStuffed);
    AddState(L"STUFFED_IDLE", new CKirbyStuffedIdle);
    AddState(L"STUFFED_RUN", new CKirbyStuffedRun);
    AddState(L"STUFFED_JUMP", new CKirbyStuffedJump);
    AddState(L"STUFFED_JUMP_FALL", new CKirbyStuffedJumpFall);
    AddState(L"STUFFED_LANDING", new CKirbyStuffedLanding);

    ChangeState(L"IDLE");
}

void CKirbyFSM::tick()
{
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        m_ChargeAccTime += DT;
    }

    if (m_bHovering)
    {
        m_HoveringAccTime += DT;
    }

    CFSMScript::tick();
}

void CKirbyFSM::ChangeAbilityCopy(AbilityCopyType _Type)
{
    ChangeState(L"CHANGE_ABILITY");
    m_CurAbility = _Type;
}

void CKirbyFSM::ChangeObjectCopy(ObjectCopyType _Type)
{
    ChangeState(L"CHANGE_OBJECT");
    m_CurObject = _Type;
}

void CKirbyFSM::SetHovering(bool _bHovering)
{
    if (m_bHovering != _bHovering)
    {
        ClearHoveringAccTime();
        PLAYERCTRL->ClearHoveringHeight();
    }
    
    m_bHovering = _bHovering;
}

void CKirbyFSM::SaveToLevelFile(FILE* _File)
{
}

void CKirbyFSM::LoadFromLevelFile(FILE* _File)
{
}
