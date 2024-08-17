#include "pch.h"
#include "CLevelFlowMgr.h"

#include <Engine/CLevelSaveLoad.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CRenderMgr.h>

#include "CPlayerMgr.h"
#include "CCameraController.h"
#include "CFadeEffectScript.h"

#include "CUIBossHPScript.h"
#include "CUIHPScript.h"

#include "CUIFlowScript.h"

CLevelFlowMgr::CLevelFlowMgr(UINT _Type)
    : CScript(_Type)
    , m_CurLevelPath{}
    , m_NextLevelPath{}
    , m_DimensionFadeEffect(nullptr)
    , m_FadeEffectScript(nullptr)
    , m_UIFlowScript(nullptr)
    , m_ToneMappingMtrl(nullptr)
    , m_pLoadingUI(nullptr)
    , m_pPlayerHP(nullptr)
    , m_pBossHP(nullptr)
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
    , m_pLoadingUI(nullptr)
    , m_pPlayerHP(nullptr)
    , m_pBossHP(nullptr)
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_NextLevelPath, "Next Level Name");
}

CLevelFlowMgr::~CLevelFlowMgr()
{
}

void CLevelFlowMgr::begin()
{
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

    // UI 초기화
    {
        m_pLoadingUI = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_Loading");
        m_pLoadingUI->SetActive(false);

        m_pPlayerHP = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_PlayerHP");

        m_pBossHP = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_BossHP");
        if (nullptr != m_pBossHP)
            m_pBossHP->SetActive(false);
    }

    // Level Start
    LevelStart();
}

void CLevelFlowMgr::tick()
{
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
    m_FadeEffectDuration = 2.f;

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

    // UI (Fade In)
    SetFadeEffect(Vec3(255.f, 0.f, 255.f), true, 0.25f, 1.25f);

    // @TODO BGM 재생
}

void CLevelFlowMgr::LevelEnd()
{
    // UI (Fade Out)
    SetFadeEffect(Vec3(255.f, 0.f, 255.f), false, 1.f, 1.25f);

    // Player UI
    TurnOffPlayerHP();

    m_bFadeEffect = true;
    m_FadeEffectAcc = 0.f;

    // @TODO BGM 종료
}

void CLevelFlowMgr::LevelExit()
{
    // UI Loading
    m_pLoadingUI->SetActive(true);

    // Level Change
    GamePlayStatic::ChangeLevelAsync(ToWstring(m_NextLevelPath), LEVEL_STATE::PLAY);
}

void CLevelFlowMgr::LevelRestart()
{
    // UI (Fade Out)
    SetFadeEffect(Vec3(255.f, 0.f, 255.f), false, 1.f, 1.25f);

    // Level Restart
    GamePlayStatic::ChangeLevelAsync(m_CurLevelPath, LEVEL_STATE::PLAY);

    // @TODO BGM 종료
}

void CLevelFlowMgr::MtrlParamUpdate()
{
    // DOF Focus Player 위치 설정
    if (nullptr != PLAYER)
    {
        static Ptr<CMaterial> pDOFMtrl = CAssetMgr::GetInst()->Load<CMaterial>(L"DOFMtrl");
        Vec3 NDCPos = PositionToNDC(PLAYER->Transform()->GetWorldPos());
        Vec2 UVPos = NDCToUV(NDCPos);
        pDOFMtrl->SetScalarParam(VEC2_0, UVPos); // Focus UV
    }
}

void CLevelFlowMgr::TurnOnBossHP()
{
    if (nullptr != m_pBossHP)
    {
        CUIBossHPScript* pScript = m_pBossHP->GetScript<CUIBossHPScript>();
        if (nullptr != pScript)
            pScript->ChangeState(HPState::Enter);
    }
}

void CLevelFlowMgr::TurnOffBossHP()
{
    if (nullptr != m_pBossHP)
    {
        m_pBossHP->SetActive(false);
    }
}

void CLevelFlowMgr::TurnOffPlayerHP()
{
    if (nullptr != m_pPlayerHP)
    {
        m_pPlayerHP->SetActive(false);
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
