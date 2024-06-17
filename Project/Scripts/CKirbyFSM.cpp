#include "pch.h"
#include "CKirbyFSM.h"

#include "CKirbyAbility.h"

CKirbyFSM::CKirbyFSM()
    : CFSMScript(KIRBYFSM)
    , m_CurAbility(nullptr)
    , m_CurObject(nullptr)
    , m_arrAbility{}
    , m_arrObject{}
{
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

        CScript* pObjCopy = _Origin.m_arrObject[i]->Clone();
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

void CKirbyFSM::begin()
{
    // State 추가

}

void CKirbyFSM::ChangeAbilityCopy(ABILITY_COPY_TYPE _Type)
{
}

void CKirbyFSM::ChangeObjectCopy(OBJECT_COPY_TYPE _Type)
{
}

void CKirbyFSM::SaveToLevelFile(FILE* _File)
{
}

void CKirbyFSM::LoadFromLevelFile(FILE* _File)
{
}
