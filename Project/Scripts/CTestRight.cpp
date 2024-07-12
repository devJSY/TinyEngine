#include "pch.h"
#include "CTestRight.h"

void CTestRight::tick()
{
	Vec3 Dir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);

	Vec3 vPos = GetOwner()->Transform()->GetLocalPos();
	vPos += Dir * DT * 200.f;

	GetOwner()->Transform()->SetLocalPos(vPos);

	if (KEY_PRESSED(KEY::UP))
	{
		ChangeState(L"FORWARD");
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

void CTestRight::Enter()
{
	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
}

void CTestRight::Exit()
{
}

CTestRight::CTestRight()
{
}

CTestRight::~CTestRight()
{
}
