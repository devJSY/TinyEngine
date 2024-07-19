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

        if (MtrlName.find(L"BodyAM") != wstring::npos)
        {
            m_DemoMeshIdx_BodyA = i;
        }
        else if (MtrlName.find(L"BodyBM") != wstring::npos)
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
}

void CKirbyObject::IdleExit()
{
    PLAYERCTRL->UnlockMove();
}

void CKirbyObject::RunEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Move"), true, false, 1.5f, 0.3f);
}

void CKirbyObject::RunEndEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Stop"), false);
}

void CKirbyObject::JumpStartEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Jump"), false);
}

void CKirbyObject::JumpFallEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Fall"));
}

void CKirbyObject::LandingEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Landing"), false);

    PLAYERCTRL->LockJump();
}

void CKirbyObject::LandingExit()
{
    PLAYERCTRL->UnlockJump();
}

void CKirbyObject::LandingEndEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"LandingEnd"), false);

    PLAYERCTRL->LockJump();
}

void CKirbyObject::LandingEndExit()
{
    PLAYERCTRL->UnlockJump();
}

void CKirbyObject::DropObject()
{
}

void CKirbyObject::DropObjectEnter()
{
    // change meshdata
    PLAYER->MeshRender()->SetMeshData(CPlayerMgr::GetPlayerMeshData());
    CPlayerMgr::ClearBodyMtrl();
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyNormal));
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Frown);

    if (PLAYERFSM->GetCurHat())
    {
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurHat(), L"Hat");
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurWeapon(), L"Weapon");
    }

    // animation & move controller
    PLAYER->Animator()->Play(KIRBYANIM(L"SpitDeform"), false, false, 1.5f);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();

    // spawn object
    CGameObject* pObj = m_OriginObject->Instantiate();
    pObj->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));    //@TODO 머지후삭제

    Vec3 BackDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -1.f;
    Vec3 InitPos = PLAYER->Transform()->GetWorldPos() + (BackDir * 300.f);
    Vec3 Force = Vec3(0.f, 1.f, 0.f) + BackDir;
    Force = Force.Normalize() * 3.f;

    pObj->Transform()->SetWorldPos(InitPos);
    pObj->Rigidbody()->AddForce(Force, ForceMode::Impulse);

    GamePlayStatic::SpawnGameObject(pObj, LAYER_DYNAMIC);
}

void CKirbyObject::DropObjectExit()
{
    CPlayerMgr::SetPlayerFace(FaceType::Normal);

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
}

void CKirbyObject::DropObjectStartEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"SpitStart"), false, false, 1.5f);

    PLAYERCTRL->AddVelocity(Vec3(0.f, 3.5f, 0.f));

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
}

void CKirbyObject::DropObjectStartExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
}

void CKirbyObject::ChangeObject()
{
    if (m_bFrmEnter && PLAYER->Animator()->GetClipFrameIndex() >= m_MeshChangeIdx)
    {
        PLAYER->GetRenderComponent()->SetMaterial(nullptr, m_DemoMeshIdx_BodyA);
        PLAYER->GetRenderComponent()->SetMaterial(CPlayerMgr::GetPlayerBodyDemoMtrl(), m_DemoMeshIdx_BodyB);

        // 뒤쪽위로 힘 가함
        Vec3 Force = Vec3(0.f, 1.f, 0.f) - PLAYER->Transform()->GetLocalDir(DIR_TYPE::FRONT);
        Force = Force.Normalize() * 10.f;
        PLAYERCTRL->AddVelocity(Force);

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
    }
    PLAYER->Animator()->Play(KIRBYANIM(L"DemoEndFirst"), false, false, 2.5f);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();
    PLAYERCTRL->LockDirection();
}

void CKirbyObject::ChangeObjectEndExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
    PLAYERCTRL->UnlockDirection();
}
