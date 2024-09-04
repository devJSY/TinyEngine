#include "pch.h"
#include "CKirbyObjDetectCollider.h"
#include "CKirbyCopyObjScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"
#include "CLevelFlowMgr.h"

CKirbyObjDetectCollider::CKirbyObjDetectCollider()
    : CScript(KIRBYOBJDETECTCOLLIDER)
    , m_bNearObj(false)
{
}

CKirbyObjDetectCollider::~CKirbyObjDetectCollider()
{
}

void CKirbyObjDetectCollider::tick()
{
    m_bNearObj = false;
    PLAYERFSM->SetNearDeformObj(false);
}

void CKirbyObjDetectCollider::OnTriggerStay(CCollider* _OtherCollider)
{
    if (_OtherCollider->GetOwner()->GetLayerIdx() != LAYER_DYNAMIC || m_bNearObj)
        return;

    wstring ObjName = _OtherCollider->GetOwner()->GetName();

    if (ObjName == L"Lightbulb" || ObjName == L"Cone" || ObjName == L"VendingMachine")
    {
        m_bNearObj = true;
        PLAYERFSM->SetNearDeformObj(true);

        // AbsorbUI On
        {
            CGameObject* ManagerObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");
            if (nullptr != ManagerObj)
            {
                CLevelFlowMgr* FlowMgrScript = ManagerObj->GetScript<CLevelFlowMgr>();
                if (nullptr != FlowMgrScript)
                    FlowMgrScript->AbsorbUIOn(_OtherCollider->GetOwner());
            }
        }
    }
}

void CKirbyObjDetectCollider::OnTriggerExit(CCollider* _OtherCollider)
{
    // AbsorbUI Off
    CGameObject* ManagerObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Manager");
    if (nullptr != ManagerObj)
    {
        CLevelFlowMgr* FlowMgrScript = ManagerObj->GetScript<CLevelFlowMgr>();
        if (nullptr != FlowMgrScript)
            FlowMgrScript->AbsorbUIOff();
    }
}

UINT CKirbyObjDetectCollider::SaveToLevelFile(FILE* _File)
{
    UINT MemroyBye = 0;
    return MemroyBye;
}

UINT CKirbyObjDetectCollider::LoadFromLevelFile(FILE* _File)
{
    UINT MemroyBye = 0;
    return MemroyBye;
}