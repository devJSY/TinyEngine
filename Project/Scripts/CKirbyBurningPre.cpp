#include "pch.h"
#include "CKirbyBurningPre.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CRenderMgr.h>

CKirbyBurningPre::CKirbyBurningPre()
    : m_SaveGravity(0.f)
{
}

CKirbyBurningPre::~CKirbyBurningPre()
{
}

void CKirbyBurningPre::tick()
{
    if (PLAYER->Animator()->IsFinish())
    {
        ChangeState(L"BURNING_START");
    }
}

void CKirbyBurningPre::Enter()
{
    // Mesh Data 바꿔주기
    //PLAYER->MeshRender()->SetMeshData(CAssetMgr::GetInst()->Load<CMeshData>(L"meshdata\\KiryDragon.mdat", L"meshdata\\KiryDragon.mdat"));

    // Kirby 표정 바꿔주기
    //Ptr<CMaterial> KirbyBody = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"material\\KiryDragon_BurningBodyC.mtrl");

    //wstring filepath = L"fbx\\Characters\\Kirby\\Base\\";
    //wstring albedo = filepath + L"KirbyEye.0" + to_wstring((UINT)FaceType::UpTail) + L".png";
    //wstring mask = filepath + L"KirbyEyeMask.0" + to_wstring((UINT)FaceType::UpTail) + L".png";
    //wstring normal = filepath + L"KirbyEyeNormal.0" + to_wstring((UINT)FaceType::UpTail) + L".png";
    //KirbyBody->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(albedo));
    //KirbyBody->SetTexParam(TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(mask));
    //KirbyBody->SetTexParam(TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(normal));

    // 애니메이션 재생
    //PLAYER->Animator()->Play(KIRBYANIM(L"BurningStart"), false, false, 1.5f);
    PLAYER->Animator()->Play(KIRBYANIM(L"DodgeFront1"), false, false, 1.5f);
    

    // Movement
    Vec3 Input = PLAYERCTRL->GetMoveDir();
    ForceDirInfo Info = {ForceDirType::DEFORM, Input};

    // 방향 강제 조정
    PLAYERCTRL->ForceDir(Info);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    PLAYERCTRL->ClearVelocityY();
    m_SaveGravity = PLAYERCTRL->GetGravity();
    PLAYERCTRL->SetGravity(0.f);
}

void CKirbyBurningPre::Exit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERCTRL->SetGravity(m_SaveGravity);
}
