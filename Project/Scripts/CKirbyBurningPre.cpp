#include "pch.h"
#include "CKirbyBurningPre.h"

#include <Engine/CAssetMgr.h>

CKirbyBurningPre::CKirbyBurningPre()
{
}

CKirbyBurningPre::~CKirbyBurningPre()
{
}

void CKirbyBurningPre::tick()
{
}

void CKirbyBurningPre::Enter()
{


    PLAYER->MeshRender()->SetMeshData(CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KiryDragon.mdat", L"meshdata\\KiryDragon.mdat"));

    PLAYER->Animator()->Play(L"BurningPre", true, false, 1.5f);



}

void CKirbyBurningPre::Exit()
{
}
