#include "pch.h"
#include "CLevelFlowMgr.h"

#include <Engine/CLevelSaveLoad.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>

#include "CPlayerMgr.h"
#include "CCameraController.h"
#include "CFadeEffectScript.h"

#include "CUIFlowScript.h"

CLevelFlowMgr::CLevelFlowMgr(UINT _Type)
    : CScript(_Type)
    , m_CurLevelPath{}
    , m_NextLevelPath{}
    , m_DimensionFadeEffect(nullptr)
    , m_FadeEffectScript(nullptr)
    , m_UIFlowScript(nullptr)
    , m_ToneMappingMtrl(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_NextLevelPath, "Next Level Name");
}

CLevelFlowMgr::CLevelFlowMgr(const CLevelFlowMgr& _Origin)
    : CScript(_Origin)
    , m_CurLevelPath{}
    , m_NextLevelPath(_Origin.m_NextLevelPath)
    , m_DimensionFadeEffect(nullptr)
    , m_FadeEffectScript(nullptr)
    , m_UIFlowScript(nullptr)
    , m_ToneMappingMtrl(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_NextLevelPath, "Next Level Name");
}

CLevelFlowMgr::~CLevelFlowMgr()
{
}

void CLevelFlowMgr::begin()
{        

    m_bIsChangedLevel = false;

    m_CurLevelPath = CLevelMgr::GetInst()->GetCurrentLevel()->GetName();

    // Effect 만들기
    m_DimensionFadeEffect =
        CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\DimensionFadeEffect.pref", L"prefab\\DimensionFadeEffect.pref")->Instantiate();

    m_DimensionFadeEffect->SetActive(false);
    GamePlayStatic::AddChildObject(GetOwner(), m_DimensionFadeEffect);

    Ptr<CPrefab> pFadeEffectPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\FadeEffect.pref", L"prefab\\FadeEffect.pref");
    if (nullptr != pFadeEffectPref)
    {
        CGameObject* pFadeEffectObj = pFadeEffectPref->Instantiate();
        m_FadeEffectScript = pFadeEffectObj->GetScript<CFadeEffectScript>();
        GamePlayStatic::AddChildObject(GetOwner(), pFadeEffectObj);
    }

    m_ToneMappingMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ToneMappingMtrl");

    // Level Start
    LevelStart();
}

void CLevelFlowMgr::tick()
{
    if (KEY_TAP(KEY::N))
    {
        LevelEnd();
    }

    if (m_bFadeEffect)
    {
        m_FadeEffectAcc += DT;

        // UI가 끝나면
        if (m_FadeEffectAcc > m_FadeEffectDuration)
        {
            // Level 전환
            m_bFadeEffect = false;
            LevelExit();
        }
    }

    // tick마다 넣어줘야 하는 Param setting
    MtrlParamUpdate();
}

void CLevelFlowMgr::LevelStart()
{
    // Post Process Enable
    CRenderMgr::GetInst()->SetEnableDOF(true);
    CRenderMgr::GetInst()->SetEnableDepthMasking(true);
    // g_Global.g_EnableSSAO = true; // Option

    // FadeEffect Timer 초기화
    m_bFadeEffect = false;
    m_FadeEffectAcc = 0.f;
    m_FadeEffectDuration = 2.5f;

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
    MainPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Main Player");

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

    // UI (Fade In)
    SetFadeEffect(Vec3(255.f, 0.f, 255.f), true, 0.25f, 1.25f);

    // @TODO BGM 재생
}

void CLevelFlowMgr::LevelEnd()
{
    // 이미 레벨 전환중일 경우 처리하지 않는다.
    if (m_bIsChangedLevel == true)
        return;

    // UI (Fade Out)
    SetFadeEffect(Vec3(255.f, 0.f, 255.f), false, 1.f, 1.25f);

    m_bIsChangedLevel = true;
    m_bFadeEffect = true;
    m_FadeEffectAcc = 0.f;

    // @TODO BGM 종료
}

void CLevelFlowMgr::LevelExit()
{
    // Loding UI 시작


    // Level Change
    GamePlayStatic::ChangeLevelAsync(ToWstring(m_NextLevelPath), LEVEL_STATE::PLAY);
}

void CLevelFlowMgr::LevelRestart()
{
    // 이미 레벨 전환중일 경우 처리하지 않는다.
    if (m_bIsChangedLevel == true)
        return;

    // UI (Fade Out)
    SetFadeEffect(Vec3(255.f, 0.f, 255.f), false, 1.f, 1.25f);

    m_bIsChangedLevel = true;
    m_bFadeEffect = true;
    m_FadeEffectAcc = 0.f;

    // 현재 레벨을 다시 시작하기 위해 NextLevelPath 를 현재레벨의 Path로 바꿔준다.
    m_NextLevelPath = ToString(m_CurLevelPath);
    // @TODO BGM 종료
}

void CLevelFlowMgr::MtrlParamUpdate()
{
    // DOF Focus Player 위치 설정
    if (nullptr != PLAYER)
    {
        static Ptr<CMaterial> pDOFMtrl = CAssetMgr::GetInst()->Load<CMaterial>(L"DOFMtrl");
        Vec3 NDCPos = PositionToNDC(PLAYER->Transform()->GetWorldPos());

        // PLAYER가 화면 밖인경우
        if (NDCPos.x < -1.f || NDCPos.y < -1.f || NDCPos.z < -1.f || NDCPos.x > 1.f || NDCPos.y > 1.f || NDCPos.z > 1.f)
        {
            pDOFMtrl->SetScalarParam(VEC2_0, Vec2(-100.f, -100.f));
        }
        else
        {
            pDOFMtrl->SetScalarParam(VEC2_0, NDCToUV(NDCPos)); // Focus UV
        }
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

void CLevelFlowMgr::SetFadeEffectColor(Vec3 _Color)
{
    if (!m_FadeEffectScript)
        return;

    Vec4 Color = Vec4(_Color.x, _Color.y, _Color.z, 255.f) / 255.f;
    m_FadeEffectScript->SetBackGroundColor(Color);
}

void CLevelFlowMgr::SetFadeEffect(Vec3 _Color, bool _bReverse, float _Duration, float _Speed)
{
    if (!m_FadeEffectScript)
        return;

    Vec4 Color = Vec4(_Color.x, _Color.y, _Color.z, 255.f) / 255.f;
    m_FadeEffectScript->SetBackGroundColor(Color);
    m_FadeEffectScript->SetReverse(_bReverse);
    m_FadeEffectScript->SetDuration(_Duration);
    m_FadeEffectScript->SetRotateSpeed(_Speed);
}

void CLevelFlowMgr::SetToneMappingParam(bool _bBloomEnable, bool _bBlendMode, float _BloomStrength, float _Threshold, float _FilterRadius,
                                        float _Exposure, float _Gamma)
{
    CRenderMgr::GetInst()->SetEnableBloom(_bBloomEnable);
    m_ToneMappingMtrl->SetScalarParam(INT_1, (int)_bBlendMode);
    m_ToneMappingMtrl->SetScalarParam(FLOAT_0, _Exposure);
    m_ToneMappingMtrl->SetScalarParam(FLOAT_1, _Gamma);
    m_ToneMappingMtrl->SetScalarParam(FLOAT_2, _BloomStrength);
    m_ToneMappingMtrl->SetScalarParam(VEC2_0, Vec2(_FilterRadius, _Threshold));
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
