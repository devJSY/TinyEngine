#include "pch.h"
#include "CKirbyObject.h"

CKirbyObject::CKirbyObject()
    : m_MeshChangeIdx(23)
    , m_bFrmEnter(true)
    , m_DemoMeshIdx_BodyA(0)
    , m_DemoMeshIdx_BodyB(0)
{
}

CKirbyObject::~CKirbyObject()
{
}

void CKirbyObject::ParseDemoMesh(Ptr<CMeshData> _pMesh)
{
    for (UINT i = 0; i < 3; ++i)
    {
        wstring MtrlName = m_DemoMesh->GetMesh()->GetIBName(i);

        if (MtrlName == L"BodyAM__BodyC")
        {
            m_DemoMeshIdx_BodyA = i;
        }
        else if (MtrlName == L"BodyBM__BodyC")
        {
            m_DemoMeshIdx_BodyB = i;
        }
    }
}

void CKirbyObject::ChangeState(const wstring& _strStateName)
{
    CFSMScript* OwnerFSM = (CFSMScript*)CPlayerMgr::GetPlayerFSM();
    assert(OwnerFSM);

    OwnerFSM->ChangeState(_strStateName);
}

void CKirbyObject::IdleEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Wait"));

    PLAYERCTRL->LockMove();
    PLAYERFSM->SetDroppable(true);
}

void CKirbyObject::IdleExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERFSM->SetDroppable(true);
}

void CKirbyObject::LandingEnter()
{
    PLAYERCTRL->LockJump();
    PLAYERFSM->SetDroppable(true);
}

void CKirbyObject::LandingExit()
{
    PLAYERCTRL->UnlockJump();
    PLAYERFSM->SetDroppable(false);
}

void CKirbyObject::LandingEndEnter()
{
    PLAYERCTRL->LockJump();
    PLAYERFSM->SetDroppable(true);
}

void CKirbyObject::LandingEndExit()
{
    PLAYERCTRL->UnlockJump();
    PLAYERFSM->SetDroppable(false);
}

void CKirbyObject::ChangeObject()
{
    if (m_bFrmEnter && PLAYER->Animator()->GetClipFrameIndex() == m_MeshChangeIdx)
    {
        PLAYER->GetRenderComponent()->SetMaterial(nullptr, m_DemoMeshIdx_BodyA);
        PLAYER->GetRenderComponent()->SetMaterial(CPlayerMgr::GetPlayerBodyDemoMtrl(), m_DemoMeshIdx_BodyB);

        m_bFrmEnter = false;
    }
}

void CKirbyObject::ChangeObjectEnter()
{
    PLAYER->MeshRender()->SetMeshData(m_DemoMesh);
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, m_DemoMeshIdx_BodyB);
    if (PLAYERFSM->GetCurHat())
    {
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurHat(), L"Hat");
    }
    PLAYER->Animator()->Play(KIRBYANIM(L"DeformFirst"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();

    m_bFrmEnter = true;
}

void CKirbyObject::ChangeObjectExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();
}

void CKirbyObject::ChangeObjectEndEnter()
{
    PLAYER->MeshRender()->SetMeshData(m_Mesh);
    if (PLAYERFSM->GetCurHat())
    {
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurHat(), L"Hat");

        PLAYERCTRL->UnlockMove();
        PLAYERCTRL->UnlockJump();
        PLAYERCTRL->UnlockDirection();
    }
    PLAYER->Animator()->Play(KIRBYANIM(L"DemoEndFirst"), false, false, 2.5f);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();
}

void CKirbyObject::ChangeObjectEndExit()
{
}
