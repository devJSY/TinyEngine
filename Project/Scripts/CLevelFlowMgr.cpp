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

#include "CEnterUIScript.h"
#include "CUIFlowScript.h"

#include "CUIHPScript.h"
#include "CKirbyDropOutUIScript.h"

CLevelFlowMgr::CLevelFlowMgr(UINT _Type)
    : CScript(_Type)
    , m_CurLevelPath{}
    , m_NextLevelPath{}
    , m_DimensionFadeEffect(nullptr)
    , m_FadeEffectScript(nullptr)
    , m_UIFlowScript(nullptr)
    , m_ToneMappingMtrl(nullptr)
    , m_RadialBlurEffect(nullptr)
    , m_BGM(nullptr)
    , m_StartBGMVolume(0.f)
    , m_EndBGMVolume(0.f)
    , m_BGMAcc(0.f)
    , m_BGMDuration(0.f)
    , m_pLoadingUI(nullptr)
    , m_pPlayerHP(nullptr)
    , m_pBossHP(nullptr)
    , m_pDropUI(nullptr)
    , m_pEnterUIScript(nullptr)
    , m_pClearUI(nullptr)
    , m_bStartLevel(false)
    , m_bStartLevelDurationValue(true)
    , m_bEnterLevel(true)
    , m_fFadeInAccTime(0.f)
    , m_fLoadingAccTime(0.f)
    , m_fFadeInWaitTime(2.3f)
    , m_bUILevel(false)
    , m_bAlreayEnterBossUI(true)
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
    , m_BGM(nullptr)
    , m_StartBGMVolume(0.f)
    , m_EndBGMVolume(0.f)
    , m_BGMAcc(0.f)
    , m_pLoadingUI(nullptr)
    , m_pEnterUIScript(nullptr)
    , m_pPlayerHP(nullptr)
    , m_pClearUI(nullptr)
    , m_pBossHP(nullptr)
    , m_pDropUI(nullptr)
    , m_bStartLevel(false)
    , m_bStartLevelDurationValue(true)
    , m_bEnterLevel(true)
    , m_fLoadingAccTime(0.f)
    , m_fFadeInAccTime(0.f)
    , m_fFadeInWaitTime(2.f)
    , m_bUILevel(false)
    , m_RadialBlurEffect(nullptr)
    , m_bAlreayEnterBossUI(true)
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

    // ==================
    //    Effect Init
    // ==================
    m_DimensionFadeEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\DimensionFadeEffect.pref")->Instantiate();

    m_DimensionFadeEffect->SetActive(false);
    GamePlayStatic::AddChildObject(GetOwner(), m_DimensionFadeEffect);

    m_RadialBlurEffect = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\RadialBlurEffect.pref")->Instantiate();

    m_RadialBlurEffect->SetActive(false);
    GamePlayStatic::AddChildObject(GetOwner(), m_RadialBlurEffect);

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
        // Loading UI 초기화
        m_pLoadingUI = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_Loading");
        if (nullptr != m_pLoadingUI)
            m_pLoadingUI->SetActive(false);

        // Player 초기화
        m_pPlayerHP = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_PlayerHP");
        if (nullptr != m_pPlayerHP)
            m_pPlayerHP->SetActive(false);

        // Boss 초기화
        m_pBossHP = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_BossHP");
        if (nullptr != m_pBossHP)
            m_pBossHP->SetActive(false);

        // Drop UI
        m_pDropUI = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_PlayerDropProgressBarUI");
        if (nullptr != m_pDropUI)
            m_pDropUI->SetActive(false);

        // Start Level Duration Value
        m_bStartLevelDurationValue = true;

        // Enter UI Script 초기화
        CGameObject* pObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_EnterScene");
        if (nullptr != pObj)
        {
            m_pEnterUIScript = pObj->GetScript<CEnterUIScript>();
        }

        m_pClearUI = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"UI_LevelClear");
        if (nullptr != m_pClearUI)
            TurnOffStageClearUI();
    }
}

void CLevelFlowMgr::tick()
{
    // 다음 레벨로 이동
    if (KEY_TAP(KEY::N) && KEY_PRESSED(KEY::LCTRL))
    {
        LevelEnd();
    }

    // 레벨 재 시작
    if (KEY_TAP(KEY::R) && KEY_PRESSED(KEY::LCTRL))
    {
        LevelRestart();
    }

    // 로비 레벨로 이동
    if (KEY_TAP(KEY::M) && KEY_PRESSED(KEY::LCTRL))
    {
        RobbyLevel();
    }

    // 스타트 레벨 UI 시작!
    if (m_bStartLevel)
    {
        if (nullptr != m_pEnterUIScript)
        {
            if (!m_pEnterUIScript->IsFinish())
            {
                PauseFadeEffect(true);
            }
            else
            {
                PauseFadeEffect(false);

                m_bStartLevel = false;
            }
        }
    }

    // Player HP UI On
    if (m_bEnterLevel)
    {
        m_fFadeInAccTime += DT_ENGINE;

        if (m_CurLevelPath == L"Level1-1-2.tLevel")
        {
            m_fFadeInWaitTime = 1.f;
        }

        // UI가 끝나면
        if (m_fFadeInAccTime > m_fFadeInWaitTime)
        {
            m_bEnterLevel = false;
            if (nullptr != m_pPlayerHP)
                m_pPlayerHP->SetActive(true);
        }
    }

    if (m_bLoadingUIWait)
    {
        m_fLoadingAccTime += DT_ENGINE;
        if (2.f <= m_fLoadingAccTime)
        {
            m_bLoadingUIWait = false;
            LevelExit();
        }
    }

    if (m_bFadeEffect)
    {
        m_FadeEffectAcc += DT_ENGINE;

        // UI가 끝나면
        if (m_FadeEffectAcc > m_FadeEffectDuration)
        {
            // Level 전환
            m_bFadeEffect = false;
            LevelLoading();
        }
    }

    if (m_bRadialBlurEffect)
    {
        m_RadialBlurAcc += DT_ENGINE;

        if (m_RadialBlurAcc > m_RadialBlurDuration)
        {
            OffRadialBlurEffect();
        }
    }

    if (m_bDimensionalFade && m_DimensionFadeEffect->IsActive())
    {
        m_DimensionalAcc += DT_ENGINE;

        if (m_CurDimensionalCoef != m_EndDimensionalCoef)
        {
            float Alpha = clamp((m_DimensionalAcc / m_DimensionalDuration), 0.f, 1.f);

            m_CurDimensionalCoef = Lerp(m_StartDimensionalCoef, m_EndDimensionalCoef, Alpha);

            if (Alpha == 1.f)
            {
                m_bDimensionalFade = false;
            }
        }
    }

    // BGM
    if (nullptr != m_BGM && m_BGMAcc <= m_BGMDuration)
    {
        m_BGMAcc += DT_ENGINE;

        float Volume = Lerp(m_StartBGMVolume, m_EndBGMVolume, m_BGMAcc / m_BGMDuration);
        GamePlayStatic::PlayBGM(m_BGM->GetRelativePath(), Volume);
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
    ResetFadeEffectTimer();

    // FadeIn 초기화
    m_bEnterLevel = true;
    m_bLoadingUIWait = false;
    m_fFadeInAccTime = 0.f;
    m_fLoadingAccTime = 0.f;
    m_fFadeInWaitTime = 1.f;

    // Radial Blur Effect Timer 초기화
    m_bRadialBlurEffect = false;
    m_RadialBlurAcc = 0.f;
    m_RadialBlurDuration = 0.f;

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

    MainPlayer->SetName(L"Main Player");
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

    // SetPlayer
    if (nullptr != m_pPlayerHP)
        m_pPlayerHP->GetScript<CUIHPScript>()->SetPlayer();

    // UI (Fade In)
    if (m_bStartLevel)
    {
        SetFadeEffect(Vec3(255.f, 0.f, 255.f), true, 1.f, 1.25f, false);
        if (nullptr != m_pEnterUIScript)
        {
            m_pEnterUIScript->ChangeState(EnterUIState::Enter);
        }
    }
    else
    {
        if (!m_bUILevel)
            SetFadeEffect(Vec3(255.f, 0.f, 255.f), true, 1.f, 1.25f, false);
    }

    if (nullptr != m_pDropUI)
        m_pDropUI->SetActive(true);
    // @TODO BGM 재생
}

void CLevelFlowMgr::LevelEnd()
{
    // 이미 레벨 전환중일 경우 처리하지 않는다.
    if (m_bIsChangedLevel == true)
        return;

    // UI (Fade Out)
    if (!m_bUILevel)
        SetFadeEffect(Vec3(255.f, 0.f, 255.f), false, 1.f, 1.25f, false);
    else if (m_CurLevelPath == L"Level1-1-2.tLevel")
        SetFadeEffect(Vec3(164.f, 44.f, 174.f), false, 1.f, 1.25f, false);

    if (m_pClearUI)
        TurnOffStageClearUI();

    // HP UI Turn Off
    TurnOffPlayerHP();
    TurnOffBossHP();
    if (nullptr != m_pDropUI)
        m_pDropUI->SetActive(false);

    // Kirby 프리팹 저장
    if (nullptr != PLAYER)
    {
        Ptr<CPrefab> MainPlayerPref = new CPrefab(PLAYER->Clone());
        MainPlayerPref->Save(L"prefab\\Main Player.pref");

        Ptr<CPrefab> CurKirbyPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\Main Player.pref");

        if (CurKirbyPref.Get())
        {
            GamePlayStatic::DeleteAsset(ASSET_TYPE::PREFAB, CurKirbyPref.Get());
        }

        // CAssetMgr::GetInst()->ReplacePrefab(MainPlayerPref, L"prefab\\Main Player.pref");
    }

    m_bIsChangedLevel = true;
    m_bFadeEffect = true;
    m_fFadeInWaitTime = 1.f;

    // FadeIn 초기화

    FadeOutBGM(1.f);
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
    TurnOffPlayerHP();
    TurnOffBossHP();

    if (L"Start Level" != m_CurLevelPath)
        SetFadeEffect(Vec3(255.f, 0.f, 255.f), false, 1.f, 1.25f, false);

    m_bIsChangedLevel = true;
    m_bFadeEffect = true;
    m_FadeEffectAcc = 0.f;

    // 현재 레벨을 다시 시작하기 위해 NextLevelPath 를 현재레벨의 Path로 바꿔준다.
    m_NextLevelPath = ToString(m_CurLevelPath);

    FadeOutBGM(1.f);
}

void CLevelFlowMgr::RobbyLevel()
{
    // 이미 레벨 전환중일 경우 처리하지 않는다.
    if (m_bIsChangedLevel == true)
        return;

    // UI (Fade Out)s
    TurnOffPlayerHP();
    TurnOffBossHP();

    SetFadeEffect(Vec3(252.f, 75.f, 129.f), false, 1.f, 1.25f, false);

    m_bIsChangedLevel = true;
    m_bFadeEffect = true;
    m_FadeEffectAcc = 0.f;

    // 현재 레벨을 다시 시작하기 위해 NextLevelPath 를 현재레벨의 Path로 바꿔준다.
    m_NextLevelPath = "Robby Level.tLevel";

    FadeOutBGM(1.f);
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
    if (nullptr != PLAYER)
    {
        if (m_RadialBlurEffect->IsActive())
        {
            static Ptr<CMaterial> pRadialBlurMtrl = CAssetMgr::GetInst()->Load<CMaterial>(L"RadialBlurMtrl");
            Vec3 NDCPos = PositionToNDC(PLAYER->Transform()->GetWorldPos());
            Vec2 UV = NDCToUV(NDCPos);
            float OnOffTime = 0.2f;
            float Radius = 20.f;
            float BlurPower = 5.f;

            // start
            if (m_RadialBlurAcc <= OnOffTime)
            {
                float t = m_RadialBlurAcc / OnOffTime;
                BlurPower *= t;
            }

            // end
            if (m_RadialBlurAcc >= m_RadialBlurDuration - OnOffTime)
            {
                float t = 1.f - (m_RadialBlurAcc - OnOffTime) / OnOffTime;
                BlurPower *= t;
            }

            pRadialBlurMtrl->SetScalarParam(VEC2_0, UV);
            pRadialBlurMtrl->SetScalarParam(FLOAT_0, Radius);
            pRadialBlurMtrl->SetScalarParam(FLOAT_1, BlurPower);
        }
    }

    if (m_DimensionFadeEffect->IsActive())
    {
        CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCamera();

        static Ptr<CMaterial> pDimensionalFadeMtrl = CAssetMgr::GetInst()->Load<CMaterial>(L"material\\DimensionFadeMtrl.mtrl");
        pDimensionalFadeMtrl->SetScalarParam(FLOAT_0, m_CurDimensionalCoef);
        if (nullptr != pMainCam)
        {
            pDimensionalFadeMtrl->SetScalarParam(FLOAT_1, pMainCam->GetFar());
        }
    }
}

void CLevelFlowMgr::TurnOnBossHP()
{
    if (nullptr != m_pBossHP)
    {
        CUIBossHPScript* pScript = m_pBossHP->GetScript<CUIBossHPScript>();
        if (nullptr != pScript)
        {
            pScript->GetOwner()->SetActive(true);
            if (m_bAlreayEnterBossUI)
            {
                m_bAlreayEnterBossUI = false;
                pScript->ChangeState(HPState::Enter);
            }
        }
    }
}

void CLevelFlowMgr::TurnOffBossHP()
{
    if (nullptr != m_pBossHP)
    {
        m_pBossHP->SetActive(false);
    }
}

void CLevelFlowMgr::TurnOnPlayerHP()
{
    if (nullptr != m_pPlayerHP)
    {
        m_pPlayerHP->SetActive(true);
    }
}

void CLevelFlowMgr::TurnOffPlayerHP()
{
    if (nullptr != m_pPlayerHP)
    {
        SetEnterLevel(false);
        m_pPlayerHP->SetActive(false);
    }
}

void CLevelFlowMgr::TurnOffStageClearUI()
{
    if (nullptr != m_pClearUI)
    {
        m_pClearUI->SetActive(false);
    }
}

void CLevelFlowMgr::TurnOnStageclearUI()
{
    if (nullptr != m_pClearUI)
    {
        m_pClearUI->SetActive(true);
    }
}

void CLevelFlowMgr::SetLoadingUIColor(Vec3 _Color)
{
    if (!m_pLoadingUI)
        return;

    Vec4 Color = Vec4(_Color.x, _Color.y, _Color.z, 255.f) / 255.f;

    vector<CGameObject*> vChilds = m_pLoadingUI->GetChildObject();

    for (size_t i = 0; i < vChilds.size(); ++i)
    {
        if (nullptr != vChilds[i])
        {
            if (vChilds[i]->GetName() == L"CircleM" || vChilds[i]->GetName() == L"CircleL" || vChilds[i]->GetName() == L"BoldCircleL")
            {
                vChilds[i]->MeshRender()->GetMaterial(0)->SetAlbedo(Color);
            }
        }
    }
}

void CLevelFlowMgr::OnDimensionFade(float _Coef)
{
    if (nullptr != m_DimensionFadeEffect)
    {
        m_DimensionFadeEffect->SetActive(true);

        m_bDimensionalFade = false;
        m_CurDimensionalCoef = _Coef;
    }
}

void CLevelFlowMgr::OnDimensionFade(float _StartCoef, float _EndCoef, float _Duration)
{
    if (nullptr != m_DimensionFadeEffect)
    {
        m_DimensionFadeEffect->SetActive(true);

        m_bDimensionalFade = true;
        m_StartDimensionalCoef = _StartCoef;
        m_EndDimensionalCoef = _EndCoef;
        m_DimensionalAcc = 0.f;
        m_DimensionalDuration = _Duration;
        m_CurDimensionalCoef = m_StartDimensionalCoef;
    }
}

void CLevelFlowMgr::OffDimensionFade()
{
    if (nullptr != m_DimensionFadeEffect)
    {
        m_DimensionFadeEffect->SetActive(false);

        m_bDimensionalFade = false;
        m_StartDimensionalCoef = 1.f;
        m_EndDimensionalCoef = 1.f;
        m_DimensionalAcc = 0.f;
        m_DimensionalDuration = 0.f;
        m_CurDimensionalCoef = 1.f;
    }
}

void CLevelFlowMgr::OnRadialBlurEffect(float _Duration, float _Radius, float _BlurPower)
{
    if (nullptr != m_RadialBlurEffect)
    {
        m_RadialBlurAcc = 0.f;
        m_RadialBlurDuration = _Duration;
        m_bRadialBlurEffect = true;

        static Ptr<CMaterial> pRadialBlurMtrl = CAssetMgr::GetInst()->Load<CMaterial>(L"RadialBlurMtrl");
        pRadialBlurMtrl->SetScalarParam(FLOAT_0, _Radius);
        pRadialBlurMtrl->SetScalarParam(FLOAT_1, _BlurPower);

        m_RadialBlurEffect->SetActive(true);
    }
}

void CLevelFlowMgr::OffRadialBlurEffect()
{
    if (nullptr != m_RadialBlurEffect)
    {
        m_RadialBlurAcc = 0.f;
        m_RadialBlurDuration = 0.f;
        m_bRadialBlurEffect = false;

        m_RadialBlurEffect->SetActive(false);
    }
}

void CLevelFlowMgr::SetRadialBlurEffect(float _Radius, float _BlurPower, Vec2 _UV)
{
    static Ptr<CMaterial> pRadialBlurMtrl = CAssetMgr::GetInst()->Load<CMaterial>(L"RadialBlurMtrl");
    pRadialBlurMtrl->SetScalarParam(FLOAT_0, _Radius);
    pRadialBlurMtrl->SetScalarParam(FLOAT_1, _BlurPower);
    pRadialBlurMtrl->SetScalarParam(VEC2_0, _UV);
}

void CLevelFlowMgr::SetFadeEffectColor(Vec3 _Color)
{
    if (!m_FadeEffectScript)
        return;

    Vec4 Color = Vec4(_Color.x, _Color.y, _Color.z, 255.f) / 255.f;
    m_FadeEffectScript->SetBackGroundColor(Color);
}

void CLevelFlowMgr::ActiveFadeEffect(bool _bEnable)
{
    if (nullptr == m_FadeEffectScript)
        return;

    m_FadeEffectScript->GetOwner()->SetActive(_bEnable);
}

void CLevelFlowMgr::PauseFadeEffect(bool _bPause)
{
    if (nullptr == m_FadeEffectScript)
        return;

    m_FadeEffectScript->SetComplete(_bPause);
}

void CLevelFlowMgr::SetFadeEffect(Vec3 _Color, bool _bReverse, float _Duration, float _Speed, bool _CenterMode)
{
    if (!m_FadeEffectScript)
        return;

    Vec4 Color = Vec4(_Color.x, _Color.y, _Color.z, 255.f) / 255.f;
    m_FadeEffectScript->SetBackGroundColor(Color);
    m_FadeEffectScript->SetReverse(_bReverse);
    m_FadeEffectScript->SetDuration(_Duration);
    m_FadeEffectScript->SetRotateSpeed(_Speed);
    m_FadeEffectScript->SetCenterMode(_CenterMode);
}

void CLevelFlowMgr::FadeInBGM(const wstring& _SoundPath, float _StartVolume, float _EndVolume, float _Duration)
{
    m_BGM = CAssetMgr::GetInst()->Load<CSound>(_SoundPath, _SoundPath);
    if (nullptr == m_BGM)
        return;

    m_StartBGMVolume = _StartVolume;
    m_EndBGMVolume = _EndVolume;
    m_BGMAcc = 0.f;
    m_BGMDuration = _Duration;
}

void CLevelFlowMgr::FadeOutBGM(float _Duration)
{
    if (nullptr == m_BGM)
        return;

    m_StartBGMVolume = m_EndBGMVolume;
    m_EndBGMVolume = 0.f;
    m_BGMAcc = 0.f;
    m_BGMDuration = _Duration;
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

void CLevelFlowMgr::LevelLoading()
{
    if (nullptr != m_pLoadingUI)
    {
        m_bLoadingUIWait = true;
        m_pLoadingUI->SetActive(true);
    }
}

void CLevelFlowMgr::SetUIDOFEffect()
{
    CRenderMgr::GetInst()->SetEnableDOF(false);
    static Ptr<CMaterial> pDOFMtrl = CAssetMgr::GetInst()->Load<CMaterial>(L"DOFMtrl");

    pDOFMtrl->SetScalarParam(VEC2_0, Vec2(-0.5f, -0.5f));

    pDOFMtrl->SetScalarParam(FLOAT_0, 1.f);
    pDOFMtrl->SetScalarParam(FLOAT_0, 3000.f);
}

void CLevelFlowMgr::TurnOnDropUI()
{
    if (nullptr != m_pDropUI)
    {
        CKirbyDropOutUIScript* pScript = m_pDropUI->GetScript<CKirbyDropOutUIScript>();
        if (pScript)
        {
            pScript->SetInteraction(true);
        }
    }
}

void CLevelFlowMgr::ActiveOffDropUI()
{
    if (nullptr != m_pDropUI)
        m_pDropUI->SetActive(false);
}

void CLevelFlowMgr::ActiveOnDropUI()
{
    if (nullptr != m_pDropUI)
        m_pDropUI->SetActive(true);
}

void CLevelFlowMgr::TurnOffDropUI()
{
    if (nullptr != m_pDropUI)
    {
        CKirbyDropOutUIScript* pScript = m_pDropUI->GetScript<CKirbyDropOutUIScript>();
        if (pScript)
        {
            pScript->SetInteraction(false);
        }
    }
}

void CLevelFlowMgr::ResetFadeEffectTimer()
{
    m_bFadeEffect = false;
    m_FadeEffectAcc = 0.f;
    m_FadeEffectDuration = 2.5f;
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
