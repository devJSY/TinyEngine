#include "pch.h"
#include "CKirbyObject.h"

CKirbyObject::CKirbyObject()
    : m_MeshChangeIdx(23)
    , m_bFrmEnter(true)
    , m_DemoMeshIdx_BodyA(0)
    , m_DemoMeshIdx_BodyB(0)
    , m_Mesh(nullptr)
    , m_DemoMesh(nullptr)
    , m_OriginObject(nullptr)
{
}

CKirbyObject::CKirbyObject(const CKirbyObject& _Origin)
    : m_DemoMeshIdx_BodyA(_Origin.m_DemoMeshIdx_BodyA)
    , m_DemoMeshIdx_BodyB(_Origin.m_DemoMeshIdx_BodyB)
    , m_MeshChangeIdx(_Origin.m_MeshChangeIdx)
    , m_WaitingAnim(_Origin.m_WaitingAnim)
    , m_bFrmEnter(_Origin.m_bFrmEnter)
    , m_Mesh(nullptr)
    , m_DemoMesh(nullptr)
    , m_OriginObject(nullptr)
{
}

CKirbyObject::~CKirbyObject()
{
}

void CKirbyObject::ParseDemoMesh(Ptr<CMeshData> _pMesh)
{
    if (_pMesh == nullptr)
        return;

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
    if (!OwnerFSM)
    {
        assert(0);
        return;
    }

    OwnerFSM->ChangeState(_strStateName);
}

void CKirbyObject::IdleEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Wait"));

    PLAYERCTRL->LockMove();
}

void CKirbyObject::IdleExit()
{
    PLAYERCTRL->UnlockMove();
}

void CKirbyObject::RunEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Move"), true, false, 1.5f, 0.3f);
}

void CKirbyObject::RunEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Stop"), false);
}

void CKirbyObject::JumpStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Jump"), false);
}

void CKirbyObject::JumpFallEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Fall"));
}

void CKirbyObject::LandingEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Landing"), false);

    PLAYERCTRL->LockJump();
}

void CKirbyObject::LandingExit()
{
    PLAYERCTRL->UnlockJump();
}

void CKirbyObject::LandingEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("LandingEnd"), false);

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
    }
    if (PLAYERFSM->GetCurHatBlade())
    {
        PLAYERFSM->GetCurHatBlade()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"material\\MetalHat_BladeC.mtrl"), 0);
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurHatBlade(), L"Hat");
    }

    if (PLAYERFSM->GetCurWeapon())
    {
        PLAYERFSM->GetCurWeapon()->SetActive(true);
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurWeapon(), L"Weapon");
    }

    CGameObject* Wing = PLAYER->GetChildObject(L"KirbyDragon");

    if (Wing != nullptr)
    {
        GamePlayStatic::AddChildObject(PLAYER, Wing, L"Wing");
    }

    // animation & move controller
    PLAYER->Animator()->Play(ANIMPREFIX("SpitDeform"), false, false, 1.5f);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();

    // spawn object
    CGameObject* pObj = m_OriginObject->Instantiate();
    Vec3 BackDir = PLAYER->Transform()->GetWorldDir(DIR_TYPE::FRONT) * -1.f;
    Vec3 InitPos = PLAYER->Transform()->GetWorldPos() + (BackDir * 60.f);
    Vec3 Force = Vec3(0.f, 1.f, 0.f) + BackDir;
    Force = Force.Normalize() * 3.f;

    Vec3 FrontDir = -BackDir;
    pObj->Transform()->SetDirection(FrontDir);

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
    PLAYER->Animator()->Play(ANIMPREFIX("SpitStart"), false, false, 1.5f);

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
    if (m_bFrmEnter && CHECK_ANIMFRM(PLAYER, m_MeshChangeIdx))
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
    PLAYER->Animator()->Play(ANIMPREFIX("DeformFirst"), false);
    PLAYER->GetRenderComponent()->SetMaterial(nullptr, m_DemoMeshIdx_BodyB);
    if (PLAYERFSM->GetCurHat())
    {
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurHat(), L"Hat");
    }
    if (PLAYERFSM->GetCurHatBlade())
    {
        PLAYERFSM->GetCurHatBlade()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"material\\MetalHat_BladeC.mtrl"), 0);
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurHatBlade(), L"Hat");
    }

    if (PLAYERFSM->GetCurWeapon())
    {
        PLAYERFSM->GetCurWeapon()->SetActive(false);
    }

    PLAYERCTRL->LockInput();
    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockJump();

    m_bFrmEnter = true;
}

void CKirbyObject::ChangeObjectExit()
{
    PLAYERCTRL->UnlockInput();
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockJump();
}

void CKirbyObject::ChangeObjectEndEnter()
{
    PLAYER->MeshRender()->SetMeshData(m_Mesh);
    if (PLAYERFSM->GetCurHat())
    {
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurHat(), L"Hat");
    }
    if (PLAYERFSM->GetCurHatBlade())
    {
        PLAYERFSM->GetCurHatBlade()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->Load<CMaterial>(L"material\\MetalHat_BladeC.mtrl"), 0);
        GamePlayStatic::AddChildObject(PLAYER, PLAYERFSM->GetCurHatBlade(), L"Hat");
    }

    PLAYER->Animator()->Play(ANIMPREFIX("DemoEndFirst"), false, false, 2.5f);

    GamePlayStatic::Play2DSound(L"sound\\wav\\HeroBasic\\Skrr.wav", 1, KIRBY_EFFECTSOUND);

    CGameObject* pStarEffect =
        CAssetMgr::GetInst()
            ->Load<CPrefab>(L"prefab\\Effect_KirbyChangeAbilityStarSpawn.pref", L"prefab\\Effect_KirbyChangeAbilityStarSpawn.pref")
            ->Instantiate();

    GamePlayStatic::SpawnGameObject(pStarEffect, pStarEffect->GetLayerIdx());

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
