#include "pch.h"
#include "CSurpriseBoardAttackScript.h"

#include "CUnitScript.h"

CSurpriseBoardAttackScript::CSurpriseBoardAttackScript()
    : CScript(SURPRISEBOARDATTACKSCRIPT)
    , m_fDamage(0.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDamage, "Damage");
}

CSurpriseBoardAttackScript::CSurpriseBoardAttackScript(const CSurpriseBoardAttackScript& Origin)
    : CScript(Origin)
    , m_fDamage(Origin.m_fDamage)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fDamage, "Damage");
}

CSurpriseBoardAttackScript::~CSurpriseBoardAttackScript()
{
}

void CSurpriseBoardAttackScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        UnitHit hit = {DAMAGE_TYPE::NORMAL, -1.f * pObj->Transform()->GetWorldDir(DIR_TYPE::FRONT), m_fDamage, 0.f, 0.f};
        pObj->GetScript<CUnitScript>()->GetDamage(hit);
    }
}

UINT CSurpriseBoardAttackScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_fDamage, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}

UINT CSurpriseBoardAttackScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_fDamage, sizeof(float), 1, _File);

    MemoryByte += sizeof(float);

    return MemoryByte;
}