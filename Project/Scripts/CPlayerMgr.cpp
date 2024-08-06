#include "pch.h"
#include "CPlayerMgr.h"
#include "CKirbyUnitScript.h"
#include "CKirbyFSM.h"
#include "CKirbyMoveController.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CGameObject* CPlayerMgr::m_PlayerObj = nullptr;
CKirbyUnitScript* CPlayerMgr::m_PlayerUnit = nullptr;
CKirbyFSM* CPlayerMgr::m_PlayerFSM = nullptr;
CKirbyMoveController* CPlayerMgr::m_PlayerController = nullptr;
CCameraController* CPlayerMgr::m_CameraController = nullptr;
Ptr<CMeshData> CPlayerMgr::m_PlayerMeshData = nullptr;
Ptr<CMaterial> CPlayerMgr::m_PlayerBodyMtrl = nullptr;
Ptr<CMaterial> CPlayerMgr::m_PlayerBodyDemoMtrl = nullptr;
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
    m_CameraController = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera")->GetScript<CCameraController>();

    m_PlayerMeshData = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\Kirby.mdat");
    m_PlayerBodyMtrl = CAssetMgr::GetInst()->Load<CMaterial>(L"material\\Kirby_BodyC.mtrl");
    m_PlayerBodyDemoMtrl = CAssetMgr::GetInst()->Load<CMaterial>(L"material\\Kirby_DeformBodyC.mtrl");


    CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Player", LAYER_PLAYER);
    SetPlayer(pPlayer);

    if (m_PlayerObj)
    {
        CGameObject* pLight = m_PlayerObj->GetChildObject(L"DeformLight PointLight");
        if (!pLight)
        {
            MessageBox(nullptr, L"Player에 DeformLight PointLight 자식이 존재하지 않습니다", L"[경고] 플레이어 세팅 오류", MB_OK);
        }
        else
        {
            pLight->SetActive(false);
        }
    }
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

    CKirbyUnitScript* pPlayerUnit = _PlayerObj->GetScript<CKirbyUnitScript>();
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

void CPlayerMgr::SetPlayerMtrl(UINT _Idx, bool _On)
{
    if (_On)
    {
        m_PlayerObj->GetRenderComponent()->SetMaterial(m_PlayerBodyMtrl, _Idx);
    }
    else
    {
        m_PlayerObj->GetRenderComponent()->SetMaterial(nullptr, _Idx);
    }
}

void CPlayerMgr::SetPlayerFace(FaceType _Type)
{
    if (m_PlayerBodyMtrl == nullptr)
        return;

    wstring filepath = L"fbx\\Characters\\Kirby\\Base\\";
    wstring albedo = filepath + L"KirbyEye.0" + to_wstring((UINT)_Type) + L".png";
    wstring mask = filepath + L"KirbyEyeMask.0" + to_wstring((UINT)_Type) + L".png";
    wstring normal = filepath + L"KirbyEyeNormal.0" + to_wstring((UINT)_Type) + L".png";
    m_PlayerBodyMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(albedo));
    m_PlayerBodyMtrl->SetTexParam(TEX_1, CAssetMgr::GetInst()->Load<CTexture>(mask));
    m_PlayerBodyMtrl->SetTexParam(TEX_2, CAssetMgr::GetInst()->Load<CTexture>(normal));
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

void CPlayerMgr::ResetBodyColliderSetting()
{
    m_PlayerObj->CharacterController()->SetCenter(Vec3(0.f, 0.77f, 0.f));
    m_PlayerObj->CharacterController()->SetHeight(1.51f);
    m_PlayerObj->CharacterController()->SetRadius(0.51f);
    m_PlayerObj->CharacterController()->SetSkinWidth(0.015f);
    m_PlayerObj->CharacterController()->SetMinMoveDistance(0.f);
    
    static CCapsuleCollider* BodyCol = m_PlayerObj->GetChildObject(L"Body Collider")->CapsuleCollider();

    BodyCol->CapsuleCollider()->SetTrigger(true);
    BodyCol->CapsuleCollider()->SetCenter(Vec3(0.f, 0.65f, 0.f));
    BodyCol->CapsuleCollider()->SetHeight(1.51f);
    BodyCol->CapsuleCollider()->SetRadius(0.51f);
}

UINT CPlayerMgr::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CPlayerMgr::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}