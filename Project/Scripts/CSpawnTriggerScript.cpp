#include "pch.h"
#include "CSpawnTriggerScript.h"
#include "CMonsterUnitScript.h"

CSpawnTriggerScript::CSpawnTriggerScript()
    : CScript(SPAWNTRIGGERSCRIPT)
    , m_pSpawnMonster{}
    , m_iSpawnMonster{}
    , m_eState(SpawnTriggerState::Active)
{
    for (int i = 0; i < 4; i++)
    {
        AddScriptParam(SCRIPT_PARAM::INT, &m_iSpawnMonster[i], "Spawn Monster" + std::to_string(i));
    }
}

CSpawnTriggerScript::CSpawnTriggerScript(const CSpawnTriggerScript& Origin)
    : CScript(Origin)
    , m_pSpawnMonster{}
    , m_iSpawnMonster{}
    , m_eState(SpawnTriggerState::Active)
{
    for (int i = 0; i < 4; i++)
    {
        AddScriptParam(SCRIPT_PARAM::INT, &m_iSpawnMonster[i], "Spawn Monster" + std::to_string(i));
    }
}

CSpawnTriggerScript::~CSpawnTriggerScript()
{
}

void CSpawnTriggerScript::begin()
{
    for (int i = 0; i < 4; i++)
    {
        m_pSpawnMonster[i] = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"SpawnMonster" + std::to_wstring(m_iSpawnMonster[i]));
    }
}

void CSpawnTriggerScript::ChangeState(SpawnTriggerState _state)
{
    ExitState();
    m_eState = _state;
    EnterState();
}

void CSpawnTriggerScript::EnterState()
{
    switch (m_eState)
    {
    case SpawnTriggerState::Active:
        break;
    case SpawnTriggerState::Spawn: {
        for (int i = 0; i < 4; i++)
        {
            // Spawn Event
            if (nullptr != m_pSpawnMonster[i])
            {
                m_pSpawnMonster[i]->SetActive(true);
                m_pSpawnMonster[i]->GetScript<CMonsterUnitScript>()->begin();
            }
            ChangeState(SpawnTriggerState::DeActive);
        }
    }
    break;
    case SpawnTriggerState::DeActive:
        break;
    case SpawnTriggerState::End:
        break;
    default:
        break;
    }
}

void CSpawnTriggerScript::ExitState()
{
}

void CSpawnTriggerScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName() && SpawnTriggerState::Active == m_eState)
    {
        ChangeState(SpawnTriggerState::Spawn);
    }
}

UINT CSpawnTriggerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    for (int i = 0; i < 4; i++)
    {
        fwrite(&m_iSpawnMonster[i], sizeof(int), 1, _File);
        MemoryByte += sizeof(int);
    }

    return MemoryByte;
}

UINT CSpawnTriggerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    for (int i = 0; i < 4; i++)
    {
        fread(&m_iSpawnMonster[i], sizeof(int), 1, _File);
        MemoryByte += sizeof(int);
    }

    return MemoryByte;
}