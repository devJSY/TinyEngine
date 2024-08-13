#include "pch.h"
#include "CLevelFlowMgr.h"

#include <Engine/CLevelSaveLoad.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>

#include "CPlayerMgr.h"
#include "CCameraController.h"

CLevelFlowMgr::CLevelFlowMgr(UINT _Type)
    : CScript(_Type)
    , m_CurLevelPath{}
    , m_NextLevelPath{}
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_NextLevelPath, "Next Level Name");
}

CLevelFlowMgr::CLevelFlowMgr(const CLevelFlowMgr& _Origin)
    : CScript(_Origin)
    , m_CurLevelPath{}
    , m_NextLevelPath(_Origin.m_NextLevelPath)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_NextLevelPath, "Next Level Name");
}

CLevelFlowMgr::~CLevelFlowMgr()
{
}

void CLevelFlowMgr::LevelStart()
{
    // Stating Point 가져오기
    CGameObject* StartingPoint = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Starting Point");

    Vec3 StartingPos = Vec3(0.f, 0.f, 0.f);
    Vec3 StartingRot = Vec3(0.f, 0.f, 0.f);

    if (StartingPoint != nullptr)
    {
        StartingPos = StartingPoint->Transform()->GetWorldPos();
        StartingRot = StartingPoint->Transform()->GetWorldRotation();
        GamePlayStatic::DestroyGameObject(StartingPoint);
    }

    // MainPlayer
    CGameObject* MainPlayer = nullptr;
    MainPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Player", LAYER_PLAYER);

    // Level에 Main Player가 없다면 직접 생성
    if (MainPlayer == nullptr)
    {
        // 이전 레벨에서 저장된 플레이어 프리팹이 있다면
        Ptr<CPrefab> PlayerPrefab = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Main Player.pref", L"prefab\\Main Player.pref");
        if (PlayerPrefab.Get() != nullptr)
        {
            MainPlayer = PlayerPrefab->Instantiate();
        }
        else
        {
            PlayerPrefab = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\Kirby.pref", L"prefab\\Kirby.pref");

            // Kirby Prefab이 없다면 assert 게임 시작 불가능
            assert(PlayerPrefab.Get());

            MainPlayer = PlayerPrefab->Instantiate();
        }

        GamePlayStatic::SpawnGameObject(MainPlayer, LAYER_PLAYER);
    }

    // Player Spawn
    MainPlayer->Transform()->SetWorldPos(StartingPos);
    MainPlayer->Transform()->SetWorldRotation(StartingRot);

    // Player 등록이 필요한 Manager들에게 Player를 등록한다.
    CPlayerMgr::SetPlayer(MainPlayer);
    CGameObject* MainCam = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Camera");
    if (MainCam)
    {
        CCameraController* CamCtrl = MainCam->GetScript<CCameraController>();
        if (CamCtrl != nullptr)
        {
            CamCtrl->SetPlayer(MainPlayer);
        }
    }

    // @TODO BGM 재생
    // @TODO UI (Fade In)
}

void CLevelFlowMgr::LevelEnd()
{
    // BGM 종료
    // UI (Fade Out)
}

void CLevelFlowMgr::LevelExit()
{

    // Level Change
    GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(ToWstring(m_NextLevelPath)), LEVEL_STATE::PLAY);
}

void CLevelFlowMgr::LevelRestart()
{
    // BGM 종료
    // UI (Fade Out)

    // Level Restart

    GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(m_CurLevelPath), LEVEL_STATE::PLAY);
}

void CLevelFlowMgr::begin()
{
    m_CurLevelPath = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();

    // Effect 만들기
    m_DimensionFadeEffect =
        CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\DimensionFadeEffect.pref", L"prefab\\DimensionFadeEffect.pref")->Instantiate();

    m_DimensionFadeEffect->SetActive(false);
    GamePlayStatic::AddChildObject(GetOwner(), m_DimensionFadeEffect);

    // Level Start
    LevelStart();
}

void CLevelFlowMgr::tick()
{
    // tick마다 넣어줘야 하는 Param setting
    MtrlParamUpdate();
}

void CLevelFlowMgr::MtrlParamUpdate()
{
    // // DOF Focus Player 위치 설정
    if (nullptr != PLAYER)
    {
        static Ptr<CMaterial> pDOFMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DOFMtrl");
        Vec3 NDCPos = PositionToNDC(PLAYER->Transform()->GetWorldPos());
        Vec2 UVPos = NDCToUV(NDCPos);
        pDOFMtrl->SetScalarParam(VEC2_0, UVPos); // Focus UV
    }
}

void CLevelFlowMgr::OnDimensionFade()
{
    if (nullptr != m_DimensionFadeEffect)
    {
        m_DimensionFadeEffect->SetActive(true);
    }
}

void CLevelFlowMgr::OffDimensionFade()
{
    if (nullptr != m_DimensionFadeEffect)
    {
        m_DimensionFadeEffect->SetActive(false);
    }
}

UINT CLevelFlowMgr::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    MemoryByte += SaveWStringToFile(ToWstring(m_NextLevelPath), _File);

    return MemoryByte;
}

UINT CLevelFlowMgr::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    wstring NextLvName;
    MemoryByte += LoadWStringFromFile(NextLvName, _File);

    m_NextLevelPath = ToString(NextLvName);

    return MemoryByte;
}
