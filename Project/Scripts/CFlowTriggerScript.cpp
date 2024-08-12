#include "pch.h"
#include "CFlowTriggerScript.h"
#include "CPlayerMgr.h"
#include "CLevelFlowMgr.h"

CFlowTriggerScript::CFlowTriggerScript()
    : CScript(FLOWTRIGGERSCRIPT)
    , m_LevelFlowMgr(nullptr)
    , m_TriggerEnterEvent(-1)
    , m_TriggerExitEvent(-1)
    , m_bDestroyFlag{false,}
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_TriggerEnterEvent, "Enter Event Idx");
    AddScriptParam(SCRIPT_PARAM::INT, &m_TriggerExitEvent, "Enter Event Idx");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bDestroyFlag[0], "Destroy (Enter)");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bDestroyFlag[1], "Destroy (Exit)");
}

CFlowTriggerScript::CFlowTriggerScript(const CFlowTriggerScript& _Origin)
    : CScript(_Origin)
    , m_TriggerEnterEvent(_Origin.m_TriggerEnterEvent)
    , m_TriggerExitEvent(_Origin.m_TriggerEnterEvent)
    , m_bDestroyFlag{_Origin.m_bDestroyFlag[0], _Origin.m_bDestroyFlag[1]}
{
}

CFlowTriggerScript::~CFlowTriggerScript()
{
}

void CFlowTriggerScript::begin()
{
    CGameObject* Manager = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager", LAYER_MANAGER);

    if (Manager)
    {
        m_LevelFlowMgr = Manager->GetScript<CLevelFlowMgr>();
    }
}

UINT CFlowTriggerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_TriggerEnterEvent, sizeof(int), 1, _File);
    fwrite(&m_TriggerExitEvent, sizeof(int), 1, _File);

    MemoryByte += sizeof(int);
    MemoryByte += sizeof(int);

    return MemoryByte;
}

UINT CFlowTriggerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_TriggerEnterEvent, sizeof(int), 1, _File);
    fread(&m_TriggerExitEvent, sizeof(int), 1, _File);

    MemoryByte += sizeof(int);
    MemoryByte += sizeof(int);

    return MemoryByte;
}

void CFlowTriggerScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    if (!m_LevelFlowMgr || m_TriggerEnterEvent == -1 || _OtherCollider->GetOwner() != PLAYER)
        return;

    m_LevelFlowMgr->TriggerEvent(m_TriggerEnterEvent);

    if (m_bDestroyFlag[0])
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}

void CFlowTriggerScript::OnTriggerExit(CCollider* _OtherCollider)
{
    if (!m_LevelFlowMgr || m_TriggerExitEvent == -1 || _OtherCollider->GetOwner() != PLAYER)
        return;

    m_LevelFlowMgr->TriggerEvent(m_TriggerExitEvent);

    if (m_bDestroyFlag[1])
    {
        GamePlayStatic::DestroyGameObject(GetOwner());
    }
}