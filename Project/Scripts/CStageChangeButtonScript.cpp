#include "pch.h"
#include "CStageChangeButtonScript.h"

#include "CLevelFlowMgr.h"

CStageChangeButtonScript::CStageChangeButtonScript()
    : CButtonScript(STAGECHANGEBUTTONSCRIPT)
    , m_vStageName{}
    , m_iCurStage(0)
    , m_iStageCount(5)
    , m_pManager(nullptr)
{
    m_vStageName.resize(m_iStageCount);
    for (UINT i = 0; i < m_iStageCount; i++)
    {
        AddScriptParam(SCRIPT_PARAM::STRING, &m_vStageName[i], "Stage" + std::to_string(i + 1));
    }
}

CStageChangeButtonScript::~CStageChangeButtonScript()
{
}

void CStageChangeButtonScript::begin()
{
    CButtonScript::begin();
    m_pManager = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");
}

void CStageChangeButtonScript::tick()
{
    CButtonScript::tick();

    // 정해진 stage가 없을 경우에는 해당 버튼 상태를 Disabled로 만든다.
    "" == m_vStageName[m_iCurStage] ? ChangeState(ButtonState::DISABLED) : ChangeState(ButtonState::SELECTED);
}

void CStageChangeButtonScript::Func()
{
    if (nullptr != m_pManager)
    {
        m_pManager->GetScript<CLevelFlowMgr>()->LevelEnd();
    }
}

UINT CStageChangeButtonScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CButtonScript::SaveToLevelFile(_File);

    fwrite(&m_iStageCount, sizeof(UINT), 1, _File);

    for (UINT i = 0; i < m_iStageCount; i++)
    {
        MemoryByte += SaveWStringToFile(ToWstring(m_vStageName[i]), _File);
    }

    MemoryByte += sizeof(UINT);

    return MemoryByte;
}

UINT CStageChangeButtonScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += CButtonScript::LoadFromLevelFile(_File);

    fread(&m_iStageCount, sizeof(UINT), 1, _File);

    for (UINT i = 0; i < m_iStageCount; i++)
    {
        wstring _StageName = L"";
        MemoryByte += LoadWStringFromFile(_StageName, _File);
        m_vStageName[i] = ToString(_StageName);
    }

    MemoryByte += sizeof(UINT);

    return MemoryByte;
}