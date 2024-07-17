#include "pch.h"
#include "CKirbyObject_Cone.h"

CKirbyObject_Cone::CKirbyObject_Cone()
    : m_bFrmEnter(true)
{
	m_Mesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyCone.mdat", L"meshdata\\KirbyCone.mdat");
    m_DemoMesh = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyConeDemo.mdat", L"meshdata\\KirbyConeDemo.mdat");
    m_Crater = CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KirbyConeCrater.mdat", L"meshdata\\KirbyConeCrater.mdat");
    
    ParseDemoMesh(m_DemoMesh);
}

CKirbyObject_Cone::~CKirbyObject_Cone()
{
}

// ===============
// Attack
// ===============
// 머금은 물체 뱉기
// - Stuffed일 때만 들어옴

void CKirbyObject_Cone::DropObject()
{
}

void CKirbyObject_Cone::DropObjectEnter()
{
}

void CKirbyObject_Cone::DropObjectExit()
{
}