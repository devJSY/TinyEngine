#include "pch.h"
#include "CKirbyObjDetectCollider.h"
#include "CKirbyCopyObjScript.h"
#include "CPlayerMgr.h"
#include "CKirbyFSM.h"

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
    }
}

UINT CKirbyObjDetectCollider::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CKirbyObjDetectCollider::LoadFromLevelFile(FILE* _File)
{
    return 0;
}