#include "pch.h"
#include "CObstacleTriggerScript.h"

#include "CSurpriseBoardScript.h"

CObstacleTriggerScript::CObstacleTriggerScript()
    : CScript(OBSTACLETRIGGERSCRIPT)
    , m_pSurPrisedObj{}
    , m_eState(CObstacleTriggerState::Wait)
    , m_iSurPrisedNum{-1}
    , m_fWaitTime(0.f)
    , m_fAccTime(0.f)
    , m_bChainAction(false)
{
    for (int i = 0; i < 2; i++)
    {
        AddScriptParam(SCRIPT_PARAM::INT, &m_iSurPrisedNum[i], "SurPriseObject" + std::to_string(i));
    }
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fWaitTime, "Wait Time");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChainAction, "Chain Action");
}

CObstacleTriggerScript::CObstacleTriggerScript(const CObstacleTriggerScript& Origin)
    : CScript(Origin)
    , m_pSurPrisedObj{}
    , m_eState(CObstacleTriggerState::Wait)
    , m_iSurPrisedNum{-1}
    , m_fWaitTime(Origin.m_fWaitTime)
    , m_fAccTime(Origin.m_fAccTime)
    , m_bChainAction(Origin.m_bChainAction)
{
    for (int i = 0; i < 2; i++)
    {
        AddScriptParam(SCRIPT_PARAM::INT, &m_iSurPrisedNum[i], "SurPriseObject" + std::to_string(i));
    }
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fWaitTime, "Wait Time");
    AddScriptParam(SCRIPT_PARAM::BOOL, &m_bChainAction, "Chain Action");
}

CObstacleTriggerScript::~CObstacleTriggerScript()
{
}

void CObstacleTriggerScript::begin()
{
    for (int i = 0; i < 2; i++)
    {
        if (-1 != m_iSurPrisedNum[i])
        {
            m_pSurPrisedObj[i] = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"SurPriseObject" + std::to_wstring(m_iSurPrisedNum[i]));
            if (nullptr != m_pSurPrisedObj[i])
            {
                if (nullptr != m_pSurPrisedObj[i]->GetScript<CSurpriseBoardScript>())
                    m_pSurPrisedObj[i]->GetScript<CSurpriseBoardScript>()->SetChainAction(m_bChainAction);
            }
        }
    }
}

void CObstacleTriggerScript::tick()
{
    switch (m_eState)
    {
    case CObstacleTriggerState::Wait:
        break;
    case CObstacleTriggerState::Stop: {
        Stop();
    }
    break;
    case CObstacleTriggerState::End:
        break;
    default:
        break;
    }
}

void CObstacleTriggerScript::ChangeState(CObstacleTriggerState _state)
{
    ExitState();
    m_eState = _state;
    EnterState();
}

void CObstacleTriggerScript::EnterState()
{
}

void CObstacleTriggerScript::ExitState()
{
    switch (m_eState)
    {
    case CObstacleTriggerState::Wait: {
        SphereCollider()->SetEnabled(false);
    }
    break;
    }
}

void CObstacleTriggerScript::Stop()
{
    m_fAccTime += DT;
    if (m_fAccTime >= m_fWaitTime)
    {
        m_fAccTime = 0.f;
        SphereCollider()->SetEnabled(true);
        ChangeState(CObstacleTriggerState::Wait);
    }
}

void CObstacleTriggerScript::OnTriggerEnter(CCollider* _OtherCollider)
{
    CGameObject* pObj = _OtherCollider->GetOwner();
    if (L"Main Player" == pObj->GetName())
    {
        for (int i = 0; i < 2; i++)
        {
            if (nullptr != m_pSurPrisedObj[i])
                m_pSurPrisedObj[i]->GetScript<CSurpriseBoardScript>()->ChangeState(SurpriseBoardState::PrePopOut);
        }
        ChangeState(CObstacleTriggerState::Stop);
    }
}

UINT CObstacleTriggerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    for (int i = 0; i < 2; i++)
    {
        fwrite(&m_iSurPrisedNum[i], sizeof(int), 1, _File);
        MemoryByte += sizeof(int);
    }
    fwrite(&m_fWaitTime, sizeof(float), 1, _File);
    fwrite(&m_bChainAction, sizeof(bool), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}

UINT CObstacleTriggerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    for (int i = 0; i < 2; i++)
    {
        fread(&m_iSurPrisedNum[i], sizeof(int), 1, _File);
        MemoryByte += sizeof(int);
    }
    fread(&m_fWaitTime, sizeof(float), 1, _File);
    fread(&m_bChainAction, sizeof(bool), 1, _File);

    MemoryByte += sizeof(float);
    MemoryByte += sizeof(bool);

    return MemoryByte;
}