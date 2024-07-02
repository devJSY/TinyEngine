#include "pch.h"
#include "CKirbyFSM.h"
#include "CKirbyAbility.h"
#include "CKirbyObject.h"

#include "CKirbyAbility_Normal.h"

CKirbyFSM::CKirbyFSM()
    : CFSMScript(KIRBYFSM)
    , m_CurAbility(nullptr)
    , m_CurObject(nullptr)
    , m_arrAbility{}
    , m_arrObject{}
    , m_ChargeAccTime(0.f)
    , m_bVacuum(false)
    , m_bStuffed(false)
{
    m_arrAbility[(UINT)ABILITY_COPY_TYPE::NORMAL] = new CKirbyAbility_Normal();
    m_CurAbility = m_arrAbility[(UINT)ABILITY_COPY_TYPE::NORMAL];
}

CKirbyFSM::CKirbyFSM(const CKirbyFSM& _Origin)
    : CFSMScript(_Origin)
    , m_CurAbility(nullptr)
    , m_CurObject(nullptr)
    , m_arrAbility{}
    , m_arrObject{}
{
    // Ability Copy 복사
    for (UINT i = 0; i < (UINT)ABILITY_COPY_TYPE::END; ++i)
    {
        if (nullptr == _Origin.m_arrAbility[i])
            continue;

        CKirbyAbility* pAbil = _Origin.m_arrAbility[i]->Clone();
        assert(pAbil);
        m_arrAbility[i] = pAbil;
        
        // 현재 능력 복사
        if (m_CurAbility == nullptr && _Origin.m_CurAbility == _Origin.m_arrAbility[i])
        {
            m_CurAbility = pAbil;
        }
    }

    // Object Copy 복사
    for (UINT i = 0; i < (UINT)OBJECT_COPY_TYPE::END; ++i)
    {
        if (nullptr == _Origin.m_arrObject[i])
            continue;

        CKirbyObject* pObjCopy = _Origin.m_arrObject[i]->Clone();
        assert(pObjCopy);
        m_arrObject[i] = pObjCopy;

        // 현재 Object Copy 복사
        if (m_CurObject == nullptr && _Origin.m_CurObject == _Origin.m_arrObject[i])
        {
            m_CurObject = pObjCopy;
        }
    }
}

CKirbyFSM::~CKirbyFSM()
{
    for (UINT i = 0; i < (UINT)ABILITY_COPY_TYPE::END; ++i)
    {
        if (m_arrAbility[i] != nullptr)
        {
            delete m_arrAbility[i];
            m_arrAbility[i] = nullptr;
        }
    }

    for (UINT i = 0; i < (UINT)OBJECT_COPY_TYPE::END; ++i)
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
    if (KEY_TAP(KEY::Q) || KEY_PRESSED(KEY::Q))
    {
        m_ChargeAccTime += DT;
    }

    CFSMScript::tick();
}

void CKirbyFSM::ChangeAbilityCopy(ABILITY_COPY_TYPE _Type)
{
    ChangeState(L"CHANGE_ABILITY");

    m_CurAbility = m_arrAbility[(UINT)_Type];
}

void CKirbyFSM::ChangeObjectCopy(OBJECT_COPY_TYPE _Type)
{
    ChangeState(L"CHANGE_OBJECT");
    m_CurObject = m_arrObject[(UINT)_Type];
}

void CKirbyFSM::SaveToLevelFile(FILE* _File)
{
}

void CKirbyFSM::LoadFromLevelFile(FILE* _File)
{
}
