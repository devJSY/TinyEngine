#include "pch.h"
#include "CBossMgr.h"
#include "CUnitScript.h"
#include "CFSMScript.h"

CGameObject* CBossMgr::m_Boss = nullptr;
CUnitScript* CBossMgr::m_BossUnit = nullptr;
CFSMScript* CBossMgr::m_BossFSM = nullptr;

CBossMgr::CBossMgr()
    : CScript(BOSSMGR)
    , m_BossName("")
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_BossName, "Boss Object Name");
}

CBossMgr::~CBossMgr()
{
}

void CBossMgr::begin()
{
    if (!m_BossName.empty())
    {
        SetBoss();
    }
}

void CBossMgr::SetBoss()
{
    CGameObject* pBoss = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(ToWstring(m_BossName), LAYER_MONSTER);

    if (!pBoss)
    {
        MessageBox(nullptr, L"존재하지 않는 게임 오브젝트입니다", L"BossMgr 등록 실패", MB_OK);
        return;
    }

    CUnitScript* pBossUnit = pBoss->GetScript<CUnitScript>();
    CFSMScript* pBossFSM = pBoss->GetScript<CFSMScript>();

    if (!pBossUnit || !pBossFSM)
    {
        MessageBox(nullptr, L"Boss에서 적절한 스크립트를 찾을 수 없습니다", L"BossMgr 등록 실패", MB_OK);
        return;
    }

    m_Boss = pBoss;
    m_BossUnit = pBossUnit;
    m_BossFSM = pBossFSM;
}

UINT CBossMgr::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += SaveWStringToFile(ToWstring(m_BossName), _File);
    return MemoryByte;
}

UINT CBossMgr::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    wstring BossName = L"";
    MemoryByte += LoadWStringFromFile(BossName, _File);
    m_BossName = ToString(BossName);

    return MemoryByte;
}