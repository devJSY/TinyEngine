#include "pch.h"
#include "CBossMgr.h"
#include "CUnitScript.h"
#include "CFSMScript.h"

CGameObject* CBossMgr::m_Boss = nullptr;
CUnitScript* CBossMgr::m_BossUnit = nullptr;
CFSMScript* CBossMgr::m_BossFSM = nullptr;

CBossMgr::CBossMgr()
    : CScript(BOSSMGR)
    , m_BossName(L"")
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
        CGameObject* pBoss = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(m_BossName, LAYER_MONSTER);
        SetBoss(pBoss);
    }
}

void CBossMgr::SetBoss(CGameObject* _BossObj)
{
    if (!_BossObj)
    {
        MessageBox(nullptr, L"존재하지 않는 게임 오브젝트입니다", L"BossMgr 등록 실패", MB_OK);
        return;
    }

    CUnitScript* pBossUnit = _BossObj->GetScript<CUnitScript>();
    CFSMScript* pBossFSM = _BossObj->GetScript<CFSMScript>();

    if (!pBossUnit || !pBossFSM)
    {
        MessageBox(nullptr, L"Boss에서 적절한 스크립트를 찾을 수 없습니다", L"BossMgr 등록 실패", MB_OK);
        return;
    }

    m_Boss = _BossObj;
    m_BossUnit = pBossUnit;
    m_BossFSM = pBossFSM;
}

UINT CBossMgr::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += SaveWStringToFile(m_BossName, _File);
    return MemoryByte;
}

UINT CBossMgr::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += LoadWStringFromFile(m_BossName, _File);
    return MemoryByte;
}