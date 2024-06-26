#include "pch.h"
#include "CPlayerMgr.h"

static CGameObject* m_PlayerObj = nullptr;
static CUnitSCript* m_PlayerUnit = nullptr;
static CKirbyFSM* m_PlayerFSM = nullptr;

CPlayerMgr::CPlayerMgr()
    : CScript(PLAYERMGR)
{
}

CPlayerMgr::~CPlayerMgr()
{
}

void CPlayerMgr::begin()
{
}

void CPlayerMgr::tick()
{
}

void CPlayerMgr::SetPlayer(CGameObject* _PlayerObj)
{
    CUnitSCript* pPlayerUnit = _PlayerObj->GetScript<CUnitSCript>();
    CKirbyFSM* pPlayerFSM = _PlayerObj->GetScript<CKirbyFSM>();

    if (!pPlayerUnit || !pPlayerFSM)
    {
        MessageBox(nullptr, L"Player에서 Unit이나 FSM Script를 찾을 수 없습니다.", L"플레이어 등록 실패", MB_OK);
        return;
    }

    m_PlayerObj = _PlayerObj;
    m_PlayerUnit = pPlayerUnit;
    m_PlayerFSM = pPlayerFSM;
}

void CPlayerMgr::SaveToLevelFile(FILE* _File)
{
}

void CPlayerMgr::LoadFromLevelFile(FILE* _File)
{
}