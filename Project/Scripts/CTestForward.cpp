#include "pch.h"
#include "CTestForward.h"

CTestForward::CTestForward()
{
}

CTestForward::~CTestForward()
{
}

void CTestForward::tick()
{
	Vec3 Dir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::FRONT);

	Vec3 vPos = GetOwner()->Transform()->GetLocalPos();
	vPos += Dir * DT * 200.f;

	GetOwner()->Transform()->SetLocalPos(vPos);

	if (KEY_PRESSED(KEY::LEFT))
	{
		ChangeState(L"LEFT");
	}
	else if (KEY_PRESSED(KEY::RIGHT))
	{
		ChangeState(L"RIGHT");
	}
	else
	{
		ChangeState(L"IDLE");
	}



}

void CTestForward::Enter()
{
	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh"));
}

void CTestForward::Exit()
{
}
