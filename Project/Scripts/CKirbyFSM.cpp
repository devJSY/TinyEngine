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
    , m_Charge(ChargeType::NONE)
    , m_LastJump(JumpType::HIGH)
    , m_bStuffed(false)
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
    , m_Charge(ChargeType::NONE)
    , m_LastJump(JumpType::HIGH)
    , m_bStuffed(false)
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
#include "CKirbyJumpEnd.h"
#include "CKirbyLanding.h"
#include "CKirbyLandingEnd.h"
#include "CKirbyAttack.h"
#include "CKirbyAttackCharge1.h"
#include "CKirbyAttackCharge1Start.h"
#include "CKirbyAttackCharge1End.h"
#include "CKirbyAttackCharge2.h"
#include "CKirbyStuffed.h"

void CKirbyFSM::begin()
{
    // State 추가
    AddState(L"IDLE", new CKirbyIdle);
    AddState(L"RUN", new CKirbyRun);
    AddState(L"RUN_START", new CKirbyRunStart);
    AddState(L"JUMP", new CKirbyJump);
    AddState(L"JUMP_START", new CKirbyJumpStart);
    AddState(L"JUMP_END", new CKirbyJumpEnd);
    AddState(L"LANDING", new CKirbyLanding);
    AddState(L"LANDING_END", new CKirbyLandingEnd);
    AddState(L"ATTACK", new CKirbyAttack);
    AddState(L"ATTACK_CHARGE1", new CKirbyAttackCharge1);
    AddState(L"ATTACK_CHARGE1_START", new CKirbyAttackCharge1Start);
    AddState(L"ATTACK_CHARGE1_END", new CKirbyAttackCharge1End);
    AddState(L"ATTACK_CHARGE2", new CKirbyAttackCharge2);
    AddState(L"STUFFED", new CKirbyStuffed);

    ChangeState(L"IDLE");
}

void CKirbyFSM::tick()
{
    if (KEY_TAP(KEY_ATK) || KEY_PRESSED(KEY_ATK))
    {
        m_ChargeAccTime += DT;
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

void CKirbyFSM::SaveToLevelFile(FILE* _File)
{
}

void CKirbyFSM::LoadFromLevelFile(FILE* _File)
{
}
