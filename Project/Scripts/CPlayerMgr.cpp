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
Ptr<CMaterial> CPlayerMgr::m_PlayerBodyMtrl = nullptr;
KirbyMeshIdx CPlayerMgr::m_PlayerMeshIdx = KirbyMeshIdx();

CPlayerMgr::CPlayerMgr()
    : CScript(PLAYERMGR)
{
}

CPlayerMgr::~CPlayerMgr()
{
}

void CPlayerMgr::begin()
{
    //@TODO 레이어
    CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Player", 3);
    m_PlayerBodyMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\Kirby_BodyC.mtrl");
    SetPlayer(pPlayer);
}

void CPlayerMgr::tick()
{
}

void CPlayerMgr::SetPlayer(CGameObject* _PlayerObj)
{
    if (!_PlayerObj)
    {
        MessageBox(nullptr, L"존재하지 않는 Player입니다", L"플레이어 등록 실패", MB_OK);
        return;
    }

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

    for (UINT i = 0; i < m_PlayerObj->GetRenderComponent()->GetMtrlCount(); ++i)
    {
        wstring MtrlName = m_PlayerObj->GetRenderComponent()->GetMesh()->GetIBName(i);
        
        if (MtrlName == L"BodyM__BodyC")
            m_PlayerMeshIdx.BodyNormal = i;
        else if (MtrlName == L"BodyBigM__BodyC")
            m_PlayerMeshIdx.BodyBig = i;
        else if (MtrlName == L"BodyVacuumM__BodyC")
            m_PlayerMeshIdx.BodyVacuum = i;
        else if (MtrlName == L"limbsM__BodyC")
            m_PlayerMeshIdx.limbs = i;
        else if (MtrlName == L"MouthNormalM__BodyC")
            m_PlayerMeshIdx.MouthNormal = i;
        else if (MtrlName == L"MouthOpenM__BodyC")
            m_PlayerMeshIdx.MouthOpen = i;
        else if (MtrlName == L"MouthSmileCloseM__BodyC")
            m_PlayerMeshIdx.MouthSmileClose = i;
        else if (MtrlName == L"MouthSmileOpenM__BodyC")
            m_PlayerMeshIdx.MouthSmileOpen = i;
        else if (MtrlName == L"MouthAngryCloseM__BodyC")
            m_PlayerMeshIdx.MouthAngryClose = i;
    }
}

void CPlayerMgr::SetPlayerMtrl(UINT _Idx)
{
    m_PlayerObj->GetRenderComponent()->SetMaterial(m_PlayerBodyMtrl, _Idx);
}

void CPlayerMgr::ClearBodyMtrl()
{
    if (!m_PlayerObj)
        return;

    m_PlayerObj->GetRenderComponent()->SetMaterial(nullptr, m_PlayerMeshIdx.BodyNormal);
    m_PlayerObj->GetRenderComponent()->SetMaterial(nullptr, m_PlayerMeshIdx.BodyBig);
    m_PlayerObj->GetRenderComponent()->SetMaterial(nullptr, m_PlayerMeshIdx.BodyVacuum);
}

void CPlayerMgr::ClearMouthMtrl()
{
    if (!m_PlayerObj)
        return;

    m_PlayerObj->GetRenderComponent()->SetMaterial(nullptr, m_PlayerMeshIdx.MouthNormal);
    m_PlayerObj->GetRenderComponent()->SetMaterial(nullptr, m_PlayerMeshIdx.MouthOpen);
    m_PlayerObj->GetRenderComponent()->SetMaterial(nullptr, m_PlayerMeshIdx.MouthSmileClose);
    m_PlayerObj->GetRenderComponent()->SetMaterial(nullptr, m_PlayerMeshIdx.MouthSmileOpen);
    m_PlayerObj->GetRenderComponent()->SetMaterial(nullptr, m_PlayerMeshIdx.MouthAngryClose);
}

void CPlayerMgr::SaveToLevelFile(FILE* _File)
{
}

void CPlayerMgr::LoadFromLevelFile(FILE* _File)
{
}