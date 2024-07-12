#include "pch.h"
#include "CTestLeft.h"

CTestLeft::CTestLeft()
{
}

CTestLeft::~CTestLeft()
{
}


void CTestLeft::tick()
{
	Vec3 Dir = GetOwner()->Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Dir.x *= -1.f;

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

void CTestLeft::Enter()
{
	GetOwner()->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh"));
}

void CTestLeft::Exit()
{
}

