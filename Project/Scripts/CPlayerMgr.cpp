#include "pch.h"
#include "CPlayerMgr.h"
#include "CUnitScript.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CGameObject* CPlayerMgr::m_PlayerObj = nullptr;
CUnitScript* CPlayerMgr::m_PlayerUnit = nullptr;
CKirbyFSM* CPlayerMgr::m_PlayerFSM = nullptr;
CKirbyMoveController* CPlayerMgr::m_PlayerController = nullptr;

CPlayerMgr::CPlayerMgr()
    : CScript(PLAYERMGR)
{
}

CPlayerMgr::~CPlayerMgr()
{
}

void CPlayerMgr::begin()
{
    //CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(L"Character")->
    CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Player");
    SetPlayer(pPlayer);
}

void CPlayerMgr::tick()
{
}

void CPlayerMgr::SetPlayer(CGameObject* _PlayerObj)
{
    CUnitScript* pPlayerUnit = _PlayerObj->GetScript<CUnitScript>();
    CKirbyFSM* pPlayerFSM = _PlayerObj->GetScript<CKirbyFSM>();
    CKirbyMoveController* pPlayerController = _PlayerObj->GetScript<CKirbyMoveController>();

    if (!pPlayerUnit || !pPlayerFSM || !pPlayerController)
    {
        MessageBox(nullptr, L"Player에서 적절한 스크립트를 찾을 수 없습니다", L"플레이어 등록 실패", MB_OK);
        return;
    }

    m_PlayerObj = _PlayerObj;
    m_PlayerUnit = pPlayerUnit;
    m_PlayerFSM = pPlayerFSM;
    m_PlayerController = pPlayerController;
}

void CPlayerMgr::SaveToLevelFile(FILE* _File)
{
}

void CPlayerMgr::LoadFromLevelFile(FILE* _File)
{
}