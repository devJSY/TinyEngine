#include "pch.h"
#include "CRenderMgr.h"

#include "CStructuredBuffer.h"

#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CEditorMgr.h"

#include "CDevice.h"
#include "components.h"

#include "CConstBuffer.h"
#include "CMRT.h"

CRenderMgr::CRenderMgr()
    : m_arrMRT{}
    , m_mainCam(nullptr)
    , m_EditorCam(nullptr)
    , m_RTCopyTex(nullptr)
    , m_IDMapTex(nullptr)
    , m_IDMapDSTex(nullptr)
    , m_PostProcessTex_LDRI(nullptr)
    , m_PostProcessTex_HDRI(nullptr)
    , m_FloatRTTex(nullptr)
    , m_Light2DBuffer(nullptr)
    , m_Light3DBuffer(nullptr)
    , m_pDebugObj(nullptr)
    , m_bShowDebugRender(true)
    , m_vecNoiseTex{}
    , m_DepthOnlyTex{}
    , m_PostEffectObj(nullptr)
    , m_bBloomEnable(false)
    , m_bloomLevels(5)
    , m_BloomRTTex_LDRI(nullptr)
    , m_BloomTextures_LDRI{}
    , m_SamplingObj(nullptr)
    , m_BlurXObj(nullptr)
    , m_BlurYObj(nullptr)
    , m_CombineObj(nullptr)
    , m_BloomTextures_HDRI{}
    , m_BloomDownObj(nullptr)
    , m_BloomUpObj(nullptr)
    , m_ToneMappingObj(nullptr)
    , m_SSAOTex(nullptr)
{
    RENDER_FUNC = &CRenderMgr::render_play;

#ifdef DISTRIBUTE
    m_bShowDebugRender = false;
#endif // DISTRIBUTE
}

CRenderMgr::~CRenderMgr()
{
    Delete_Array(m_arrMRT);

    if (nullptr != m_pDebugObj)
    {
        delete m_pDebugObj;
        m_pDebugObj = nullptr;
    }

    if (nullptr != m_Light2DBuffer)
    {
        delete m_Light2DBuffer;
        m_Light2DBuffer = nullptr;
    }

    if (nullptr != m_Light3DBuffer)
    {
        delete m_Light3DBuffer;
        m_Light3DBuffer = nullptr;
    }

    if (nullptr != m_SamplingObj)
    {
        delete m_SamplingObj;
        m_SamplingObj = nullptr;
    }

    if (nullptr != m_BlurXObj)
    {
        delete m_BlurXObj;
        m_BlurXObj = nullptr;
    }

    if (nullptr != m_BlurYObj)
    {
        delete m_BlurYObj;
        m_BlurYObj = nullptr;
    }

    if (nullptr != m_CombineObj)
    {
        delete m_CombineObj;
        m_CombineObj = nullptr;
    }

    if (nullptr != m_BloomDownObj)
    {
        delete m_BloomDownObj;
        m_BloomDownObj = nullptr;
    }

    if (nullptr != m_BloomUpObj)
    {
        delete m_BloomUpObj;
        m_BloomUpObj = nullptr;
    }

    if (nullptr != m_ToneMappingObj)
    {
        delete m_ToneMappingObj;
        m_ToneMappingObj = nullptr;
    }

    if (nullptr != m_PostEffectObj)
    {
        delete m_PostEffectObj;
        m_PostEffectObj = nullptr;
    }
}

void CRenderMgr::render()
{
    render_Clear(Vec4(0.f, 0.f, 0.f, 1.f));

    UpdateData();

    if (nullptr != m_mainCam)
    {
        // Depth Only Pass
        m_mainCam->SortShadowMapObject(MOBILITY_TYPE::STATIC | MOBILITY_TYPE::MOVABLE);
        m_mainCam->render_DepthOnly(m_DepthOnlyTex);
    }

    // Dynamic Shadow Depth Map
    render_DynamicShadowDepth();

    // Rendering
    (this->*RENDER_FUNC)();

    // Debug
    render_debug();

    Clear();
}

void CRenderMgr::render_Clear(const Vec4& Color)
{
    // MRT Clear
    for (UINT i = 0; i < (UINT)MRT_TYPE::END; i++)
    {
        if (nullptr != m_arrMRT[i])
        {
            m_arrMRT[i]->Clear();
        }
    }

    CONTEXT->ClearRenderTargetView(m_IDMapTex->GetRTV().Get(), Color);
    CONTEXT->ClearDepthStencilView(m_IDMapDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    CONTEXT->ClearRenderTargetView(m_PostProcessTex_LDRI->GetRTV().Get(), Color);
    CONTEXT->ClearRenderTargetView(m_PostProcessTex_HDRI->GetRTV().Get(), Color);

    CONTEXT->ClearDepthStencilView(m_DepthOnlyTex->GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

    CONTEXT->ClearRenderTargetView(m_BloomRTTex_LDRI->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 1.f));

    for (UINT i = 0; i < m_BloomTextures_LDRI.size(); i++)
    {
        CONTEXT->ClearRenderTargetView(m_BloomTextures_LDRI[i]->GetRTV().Get(), Color);
    }

    for (UINT i = 0; i < m_BlurTextures.size(); i++)
    {
        CONTEXT->ClearRenderTargetView(m_BlurTextures[i]->GetRTV().Get(), Color);
    }

    for (UINT i = 0; i < m_BloomTextures_HDRI.size(); i++)
    {
        CONTEXT->ClearRenderTargetView(m_BloomTextures_HDRI[i]->GetRTV().Get(), Color);
    }
}

void CRenderMgr::render_play()
{
    if (m_vecCam.empty())
        return;

    m_mainCam = m_vecCam[0];

    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        m_vecCam[i]->SortObject();
        m_vecCam[i]->render();
    }
}

void CRenderMgr::render_editor()
{
    if (nullptr == m_EditorCam)
        return;

    m_mainCam = m_EditorCam;

    m_EditorCam->SortObject();
    m_EditorCam->render();
}

void CRenderMgr::render_debug()
{
    if (nullptr == m_mainCam || !m_bShowDebugRender)
    {
        m_DbgShapeInfo.clear();
        return;
    }

    g_Transform.matView = m_mainCam->GetViewMat();
    g_Transform.matProj = m_mainCam->GetProjMat();

    list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
    for (; iter != m_DbgShapeInfo.end();)
    {
        // 매쉬 설정
        m_pDebugObj->MeshRender()->SetMesh((*iter).pMesh);

        // 머테리얼 설정
        Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl");
        pMtrl->SetScalarParam(VEC4_0, (*iter).vColor);

        // Depth 옵션 설정
        DS_TYPE PrevDSType = pMtrl->GetShader()->GetDSType();
        if ((*iter).bDepthTest)
        {
            pMtrl->GetShader()->SetDSType(DS_TYPE::LESS);
        }

        // Topology 설정
        D3D11_PRIMITIVE_TOPOLOGY PrevTopology = pMtrl->GetShader()->GetTopology();
        if (!(DEBUG_SHAPE::RECT == (*iter).eShape || DEBUG_SHAPE::CIRCLE == (*iter).eShape || DEBUG_SHAPE::CONE == (*iter).eShape))
        {
            pMtrl->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        }

        // 지정된 월드행렬로 설정
        m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);

        m_pDebugObj->render(pMtrl);

        pMtrl->GetShader()->SetDSType(PrevDSType);
        pMtrl->GetShader()->SetTopology(PrevTopology);

        // Duration Check
        (*iter).fLifeTime += DT;
        if ((*iter).fDuration <= (*iter).fLifeTime)
        {
            iter = m_DbgShapeInfo.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void CRenderMgr::render_postprocess_LDRI()
{
    if (m_bBloomEnable)
    {
        // 첫 샘플링만 Threshold 적용
        Ptr<CMaterial> pSamplingMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SamplingMtrl");
        float Threshold = pSamplingMtrl->GetMtrlConst().arrFloat[0];

        // Down Sampling
        for (int i = 0; i < m_bloomLevels - 1; i++)
        {
            if (i == 0)
                m_SamplingObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_BloomRTTex_LDRI);
            else
            {
                m_SamplingObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_BloomTextures_LDRI[i - 1]);
                pSamplingMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, 0.f);
            }

            CDevice::GetInst()->SetViewport((float)m_BloomTextures_LDRI[i]->GetWidth(), (float)m_BloomTextures_LDRI[i]->GetHeight());
            CONTEXT->OMSetRenderTargets(1, m_BloomTextures_LDRI[i]->GetRTV().GetAddressOf(), NULL);
            m_SamplingObj->render();
            CTexture::Clear(0);
        }

        // UP Sampling + Blur
        for (int i = 0; i < m_bloomLevels - 1; i++)
        {
            int level = m_bloomLevels - 2 - i;

            // Blur X
            m_BlurXObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_BloomTextures_LDRI[level]);
            CDevice::GetInst()->SetViewport((float)m_BlurTextures[level]->GetWidth(), (float)m_BlurTextures[level]->GetHeight());
            CONTEXT->OMSetRenderTargets(1, m_BlurTextures[level]->GetRTV().GetAddressOf(), NULL);
            m_BlurXObj->render();
            CTexture::Clear(0);

            // Blur Y
            m_BlurYObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_BlurTextures[level]);
            CDevice::GetInst()->SetViewport((float)m_BloomTextures_LDRI[level]->GetWidth(), (float)m_BloomTextures_LDRI[level]->GetHeight());
            CONTEXT->OMSetRenderTargets(1, m_BloomTextures_LDRI[level]->GetRTV().GetAddressOf(), NULL);
            m_BlurYObj->render();
            CTexture::Clear(0);

            // Up Sampling
            m_SamplingObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_BloomTextures_LDRI[level]);

            if (i == m_bloomLevels - 2)
            {
                CDevice::GetInst()->SetViewport((float)m_BloomRTTex_LDRI->GetWidth(), (float)m_BloomRTTex_LDRI->GetHeight());
                CONTEXT->OMSetRenderTargets(1, m_BloomRTTex_LDRI->GetRTV().GetAddressOf(), NULL);
            }
            else
            {
                CDevice::GetInst()->SetViewport((float)m_BloomTextures_LDRI[level - 1]->GetWidth(),
                                                (float)m_BloomTextures_LDRI[level - 1]->GetHeight());
                CONTEXT->OMSetRenderTargets(1, m_BloomTextures_LDRI[level - 1]->GetRTV().GetAddressOf(), NULL);
            }

            m_SamplingObj->render();
            CTexture::Clear(0);
        }

        // 원본 Threshold 설정
        pSamplingMtrl->SetScalarParam(SCALAR_PARAM::FLOAT_0, Threshold);

        // Combine
        CopyRTTexToRTCopyTex();
        m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
        m_CombineObj->render();
        CTexture::Clear(0);
        CTexture::Clear(1);
    }
}

void CRenderMgr::render_postprocess_HDRI()
{
    // =================
    // PostEffect        RTV(PostProcess), SRV(floatRTTex DepthOnlyTex)
    // =================
    CONTEXT->OMSetRenderTargets(1, m_PostProcessTex_HDRI->GetRTV().GetAddressOf(), NULL);
    m_PostEffectObj->render();
    CTexture::Clear(0);
    CTexture::Clear(1);
    CONTEXT->CopyResource(m_FloatRTTex->GetTex2D().Get(), m_PostProcessTex_HDRI->GetTex2D().Get());

    // =================
    // Bloom
    // =================
    if (m_bBloomEnable)
    {
        m_arrMRT[(UINT)MRT_TYPE::HDRI]->OMSet();
        CopyToPostProcessTex_HDRI();

        for (int i = 0; i < m_BloomTextures_HDRI.size(); i++)
        {
            if (i == 0)
                m_BloomDownObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_PostProcessTex_HDRI);
            else
                m_BloomDownObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_BloomTextures_HDRI[i - 1]);

            CDevice::GetInst()->SetViewport((float)m_BloomTextures_HDRI[i]->GetWidth(), (float)m_BloomTextures_HDRI[i]->GetHeight());
            CONTEXT->OMSetRenderTargets(1, m_BloomTextures_HDRI[i]->GetRTV().GetAddressOf(), NULL);
            m_BloomDownObj->render();
            CTexture::Clear(0);
        }
        for (int i = 0; i < m_BloomTextures_HDRI.size(); i++)
        {
            int level = m_bloomLevels - 2 - i;
            m_BloomUpObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_BloomTextures_HDRI[level]);
            if (i == m_bloomLevels - 2)
            {
                CDevice::GetInst()->SetViewport((float)m_PostProcessTex_HDRI->GetWidth(), (float)m_PostProcessTex_HDRI->GetHeight());
                CONTEXT->OMSetRenderTargets(1, m_PostProcessTex_HDRI->GetRTV().GetAddressOf(), NULL);
            }
            else
            {
                CDevice::GetInst()->SetViewport((float)m_BloomTextures_HDRI[level - 1]->GetWidth(),
                                                (float)m_BloomTextures_HDRI[level - 1]->GetHeight());
                CONTEXT->OMSetRenderTargets(1, m_BloomTextures_HDRI[level - 1]->GetRTV().GetAddressOf(), NULL);
            }
            m_BloomUpObj->render();
            CTexture::Clear(0);
        }
    }

    // =================
    // Tone Mapping + Bloom Combine
    // =================
    m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
    m_ToneMappingObj->render();
    CTexture::Clear(0);
    CTexture::Clear(1);
}

void CRenderMgr::render_DynamicShadowDepth()
{
    for (int i = 0; i < m_vecLight3D.size(); i++)
    {
        int ShadowIndex = m_vecLight3D[i]->GetShadowIdx();
        if (0 > ShadowIndex)
            continue;

        // Rendering
        m_vecLight3D[i]->render_ShadowDepth(MOBILITY_TYPE::MOVABLE);

        // Bind
        if (1 == ShadowIndex)
        {
            m_vecLight3D[i]->GetDepthMapTex()->UpdateData(23);
        }
        else if (2 == ShadowIndex)
        {
            m_vecLight3D[i]->GetDepthMapTex()->UpdateData(24);
        }
        else if (3 == ShadowIndex)
        {
            m_vecLight3D[i]->GetDepthMapTex()->UpdateData(25);
        }
    }
}

void CRenderMgr::UpdateData()
{
    // GlobalData 에 광원 개수정보 세팅
    g_Global.g_Light2DCount = (UINT)m_vecLight2D.size();
    g_Global.g_Light3DCount = (UINT)m_vecLight3D.size();

    // 메인 카메라 위치 등록
    if (nullptr != m_mainCam)
    {
        g_Global.g_eyeWorld = m_mainCam->Transform()->GetWorldPos();
    }
    else
    {
        g_Global.g_eyeWorld = Vec3();
    }

    // 전역 상수 데이터 바인딩
    CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
    pGlobalBuffer->SetData(&g_Global);
    pGlobalBuffer->UpdateData();
    pGlobalBuffer->UpdateData_CS();

    // Light2D
    static vector<tLightInfo> vecLight2DInfo;

    for (UINT i = 0; i < m_vecLight2D.size(); ++i)
    {
        const tLightInfo& info = m_vecLight2D[i]->GetLightInfo();
        vecLight2DInfo.push_back(info);
    }

    m_Light2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
    m_Light2DBuffer->UpdateData(13);

    vecLight2DInfo.clear();

    // Light3D
    static vector<tLightInfo> vecLight3DInfo;

    // 그림자 적용 광원 최대갯수
    const int dynamicShadowMaxCount = 3;
    int ShadowIdx = 1;

    for (UINT i = 0; i < m_vecLight3D.size(); ++i)
    {
        m_vecLight3D[i]->SetShadowIdx(-1); // 초기화

        if (ShadowIdx <= dynamicShadowMaxCount && MOBILITY_TYPE::MOVABLE == m_vecLight3D[i]->Transform()->GetMobilityType())
        {
            m_vecLight3D[i]->SetShadowIdx(ShadowIdx);
            ShadowIdx++;
        }

        const tLightInfo& info = m_vecLight3D[i]->GetLightInfo();
        vecLight3DInfo.push_back(info);
    }

    m_Light3DBuffer->SetData(vecLight3DInfo.data(), (UINT)vecLight3DInfo.size());
    m_Light3DBuffer->UpdateData(14);

    vecLight3DInfo.clear();
}

void CRenderMgr::Clear()
{
    m_vecLight2D.clear();
    m_vecLight3D.clear();

    // Light DepthMap Clear
    CTexture::Clear(23);
    CTexture::Clear(24);
    CTexture::Clear(25);
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Idx)
{
    if (m_vecCam.size() <= _Idx + 1)
    {
        m_vecCam.resize(_Idx + 1);
    }

    m_vecCam[_Idx] = _Cam;
}

void CRenderMgr::ActiveEditorMode(bool _bActive)
{
    if (_bActive)
        RENDER_FUNC = &CRenderMgr::render_editor;
    else
        RENDER_FUNC = &CRenderMgr::render_play;
}

CCamera* CRenderMgr::GetCamera(int _Idx) const
{
    if (m_vecCam.size() <= _Idx)
        return nullptr;

    return m_vecCam[_Idx];
}

void CRenderMgr::CopyRTTexToRTCopyTex()
{
    Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::CopyToPostProcessTex_LDRI()
{
    Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_PostProcessTex_LDRI->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::CopyToPostProcessTex_HDRI()
{
    CONTEXT->CopyResource(m_PostProcessTex_HDRI->GetTex2D().Get(), m_FloatRTTex->GetTex2D().Get());
}

void CRenderMgr::CreateRTCopyTex(Vec2 Resolution)
{
    m_RTCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                      D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
}

void CRenderMgr::CreatePostProcessTex(Vec2 Resolution)
{
    m_PostProcessTex_HDRI =
        CAssetMgr::GetInst()->CreateTexture(L"PostProessTex_HDRI", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R16G16B16A16_FLOAT,
                                            D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
    m_PostProcessTex_LDRI =
        CAssetMgr::GetInst()->CreateTexture(L"PostProessTex_LDRI", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                            D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
}

void CRenderMgr::CreateBloomTextures(Vec2 Resolution)
{
    // LDRI
    m_BloomRTTex_LDRI = nullptr;
    m_BloomTextures_LDRI.clear();

    m_BloomRTTex_LDRI =
        CAssetMgr::GetInst()->CreateTexture(L"LDRI_BloomRenderTexture", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                            D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);

    for (int i = 0; i < m_bloomLevels - 1; i++)
    {
        int div = int(pow(2, 1 + i));
        m_BloomTextures_LDRI.push_back(CAssetMgr::GetInst()->CreateTexture(
            L"LDRI_BloomTexture " + std::to_wstring(i), UINT(Resolution.x / div), UINT(Resolution.y / div), DXGI_FORMAT_R8G8B8A8_UNORM,
            D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT));
        m_BlurTextures.push_back(CAssetMgr::GetInst()->CreateTexture(L"BlurTexture " + std::to_wstring(i), UINT(Resolution.x / div),
                                                                     UINT(Resolution.y / div), DXGI_FORMAT_R8G8B8A8_UNORM,
                                                                     D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT));
    }

    // HDRI
    m_BloomTextures_HDRI.clear();

    for (int i = 0; i < m_bloomLevels - 1; i++)
    {
        int div = int(pow(2, 1 + i));
        m_BloomTextures_HDRI.push_back(CAssetMgr::GetInst()->CreateTexture(
            L"HDRI_BloomTexture " + std::to_wstring(i), UINT(Resolution.x / div), UINT(Resolution.y / div), DXGI_FORMAT_R16G16B16A16_FLOAT,
            D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT));
    }
}

void CRenderMgr::CreateMRT(Vec2 Resolution)
{
    // =============
    // SwapChain MRT
    // =============
    {
        Ptr<CTexture> arrRTTex[1] = {CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex")};
        Vec4 arrClear[1] = {Vec4(0.f, 0.f, 0.f, 1.f)};
        Ptr<CTexture> DSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

        m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;
        m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(arrRTTex, arrClear, 1, DSTex);
    }

    // =============
    // HDRI MRT
    // =============
    {
        Ptr<CTexture> arrRTTex[1] = {CAssetMgr::GetInst()->FindAsset<CTexture>(L"FloatRenderTargetTexture")};
        Vec4 arrClear[1] = {Vec4(0.f, 0.f, 0.f, 1.f)};
        Ptr<CTexture> DSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

        m_arrMRT[(UINT)MRT_TYPE::HDRI] = new CMRT;
        m_arrMRT[(UINT)MRT_TYPE::HDRI]->Create(arrRTTex, arrClear, 1, DSTex);
    }

    // ============
    // Deferred MRT
    // ============
    {
        Ptr<CTexture> arrRTTex[8] = {
            CAssetMgr::GetInst()->CreateTexture(L"AlbedoTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
            CAssetMgr::GetInst()->CreateTexture(L"PositionTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R32G32B32A32_FLOAT,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
            CAssetMgr::GetInst()->CreateTexture(L"NormalTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R32G32B32A32_FLOAT,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
            CAssetMgr::GetInst()->CreateTexture(L"TangentTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R32G32B32A32_FLOAT,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
            CAssetMgr::GetInst()->CreateTexture(L"BitangentTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R32G32B32A32_FLOAT,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
            CAssetMgr::GetInst()->CreateTexture(L"EmissiveTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R32G32B32A32_FLOAT,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
            CAssetMgr::GetInst()->CreateTexture(L"MetallicRoughnessTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
            CAssetMgr::GetInst()->CreateTexture(L"AmbientOcclusionTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
        };

        Vec4 arrClearColor[8] = {
            Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f),
            Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f),
        };

        Ptr<CTexture> DSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

        m_arrMRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;
        m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->Create(arrRTTex, arrClearColor, 8, DSTex);

        Ptr<CMaterial> pPBRDirLightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredDirLightingMtrl");
        pPBRDirLightMtrl->SetTexParam(TEX_PARAM::TEX_0, arrRTTex[0]);
        pPBRDirLightMtrl->SetTexParam(TEX_PARAM::TEX_1, arrRTTex[1]);
        pPBRDirLightMtrl->SetTexParam(TEX_PARAM::TEX_2, arrRTTex[2]);
        pPBRDirLightMtrl->SetTexParam(TEX_PARAM::TEX_3, arrRTTex[6]);
        pPBRDirLightMtrl->SetTexParam(TEX_PARAM::TEX_4, arrRTTex[7]);

        Ptr<CMaterial> pPBRPointLightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredPointLightingMtrl");
        pPBRPointLightMtrl->SetTexParam(TEX_PARAM::TEX_0, arrRTTex[0]);
        pPBRPointLightMtrl->SetTexParam(TEX_PARAM::TEX_1, arrRTTex[1]);
        pPBRPointLightMtrl->SetTexParam(TEX_PARAM::TEX_2, arrRTTex[2]);
        pPBRPointLightMtrl->SetTexParam(TEX_PARAM::TEX_3, arrRTTex[6]);
        pPBRPointLightMtrl->SetTexParam(TEX_PARAM::TEX_4, arrRTTex[7]);

        Ptr<CMaterial> pPBRSpotLightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredSpotLightingMtrl");
        pPBRSpotLightMtrl->SetTexParam(TEX_PARAM::TEX_0, arrRTTex[0]);
        pPBRSpotLightMtrl->SetTexParam(TEX_PARAM::TEX_1, arrRTTex[1]);
        pPBRSpotLightMtrl->SetTexParam(TEX_PARAM::TEX_2, arrRTTex[2]);
        pPBRSpotLightMtrl->SetTexParam(TEX_PARAM::TEX_3, arrRTTex[6]);
        pPBRSpotLightMtrl->SetTexParam(TEX_PARAM::TEX_4, arrRTTex[7]);
    }

    // ============
    // Deferred Phong MRT
    // ============
    {
        Ptr<CTexture> arrRTTex[8] = {
            CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex"),
            CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"),
            CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"),
            CAssetMgr::GetInst()->FindAsset<CTexture>(L"TangentTargetTex"),
            CAssetMgr::GetInst()->FindAsset<CTexture>(L"BitangentTargetTex"),
            CAssetMgr::GetInst()->FindAsset<CTexture>(L"EmissiveTargetTex"),
            CAssetMgr::GetInst()->CreateTexture(L"DiffuseTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
            CAssetMgr::GetInst()->CreateTexture(L"SpecularTargetTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT),
        };

        Vec4 arrClearColor[8] = {Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f),
                                 Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f), Vec4(0.f, 0.f, 0.f, 1.f)};

        Ptr<CTexture> DSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

        m_arrMRT[(UINT)MRT_TYPE::DEFERRED_PHONG] = new CMRT;
        m_arrMRT[(UINT)MRT_TYPE::DEFERRED_PHONG]->Create(arrRTTex, arrClearColor, 8, DSTex);

        Ptr<CMaterial> pDirLightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLight_deferredMtrl");
        pDirLightMtrl->SetTexParam(TEX_PARAM::TEX_0, arrRTTex[1]);
        pDirLightMtrl->SetTexParam(TEX_PARAM::TEX_1, arrRTTex[2]);
        pDirLightMtrl->SetTexParam(TEX_PARAM::TEX_2, arrRTTex[6]);
        pDirLightMtrl->SetTexParam(TEX_PARAM::TEX_3, arrRTTex[7]);

        Ptr<CMaterial> pPointLightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLight_deferredMtrl");
        pPointLightMtrl->SetTexParam(TEX_PARAM::TEX_0, arrRTTex[1]);
        pPointLightMtrl->SetTexParam(TEX_PARAM::TEX_1, arrRTTex[2]);
        pPointLightMtrl->SetTexParam(TEX_PARAM::TEX_2, arrRTTex[6]);
        pPointLightMtrl->SetTexParam(TEX_PARAM::TEX_3, arrRTTex[7]);

        Ptr<CMaterial> pSpotLightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLight_deferredMtrl");
        pSpotLightMtrl->SetTexParam(TEX_PARAM::TEX_0, arrRTTex[1]);
        pSpotLightMtrl->SetTexParam(TEX_PARAM::TEX_1, arrRTTex[2]);
        pSpotLightMtrl->SetTexParam(TEX_PARAM::TEX_2, arrRTTex[6]);
        pSpotLightMtrl->SetTexParam(TEX_PARAM::TEX_3, arrRTTex[7]);
    }

    // =========
    // Decal MRT
    // =========
    {
        Ptr<CTexture> pRTTex[3] = {
            CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex"),
            CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"),
            CAssetMgr::GetInst()->FindAsset<CTexture>(L"EmissiveTargetTex"),
        };

        Vec4 arrClearColor[3] = {
            Vec4(0.f, 0.f, 0.f, 1.f),
            Vec4(0.f, 0.f, 0.f, 1.f),
            Vec4(0.f, 0.f, 0.f, 1.f),
        };

        m_arrMRT[(UINT)MRT_TYPE::DECAL] = new CMRT;
        m_arrMRT[(UINT)MRT_TYPE::DECAL]->Create(pRTTex, arrClearColor, 3, nullptr);

        Ptr<CMaterial> pDecalMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DecalMtrl");
        pDecalMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
        pDecalMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->CreateTexture(
                                                      L"NormalTargetCopyTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R32G32B32A32_FLOAT,
                                                      D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT));
        pDecalMtrl->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"TangentTargetTex"));
        pDecalMtrl->SetTexParam(TEX_PARAM::TEX_3, CAssetMgr::GetInst()->FindAsset<CTexture>(L"BitangentTargetTex"));
    }

    // ============
    // Light MRT
    // ============
    {
        Ptr<CTexture> arrRTTex[1] = {CAssetMgr::GetInst()->CreateTexture(L"LightRadianceTargetTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                                                         DXGI_FORMAT_R8G8B8A8_UNORM,
                                                                         D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT)};

        Vec4 arrClearColor[1] = {Vec4(0.f, 0.f, 0.f, 1.f)};

        m_arrMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
        m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Create(arrRTTex, arrClearColor, 1, nullptr);

        // PBR
        Ptr<CMaterial> pPBRDeferredMergeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"UnrealPBRDeferredMergeMtrl");
        pPBRDeferredMergeMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex"));
        pPBRDeferredMergeMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
        pPBRDeferredMergeMtrl->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));
        pPBRDeferredMergeMtrl->SetTexParam(TEX_PARAM::TEX_3, CAssetMgr::GetInst()->FindAsset<CTexture>(L"EmissiveTargetTex"));
        pPBRDeferredMergeMtrl->SetTexParam(TEX_PARAM::TEX_4, CAssetMgr::GetInst()->FindAsset<CTexture>(L"MetallicRoughnessTargetTex"));
        pPBRDeferredMergeMtrl->SetTexParam(TEX_PARAM::TEX_5, CAssetMgr::GetInst()->FindAsset<CTexture>(L"AmbientOcclusionTargetTex"));
        pPBRDeferredMergeMtrl->SetTexParam(TEX_PARAM::TEX_6, arrRTTex[0]);

        // Phong
        Ptr<CMaterial> pMerge_DeferredMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Merge_DeferredMtrl");
        pMerge_DeferredMtrl->SetTexParam(TEX_PARAM::TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"AlbedoTargetTex"));
        pMerge_DeferredMtrl->SetTexParam(TEX_PARAM::TEX_1, arrRTTex[0]);
        pMerge_DeferredMtrl->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
        pMerge_DeferredMtrl->SetTexParam(TEX_PARAM::TEX_3, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));
        pMerge_DeferredMtrl->SetTexParam(TEX_PARAM::TEX_4, CAssetMgr::GetInst()->FindAsset<CTexture>(L"DiffuseTargetTex"));
        pMerge_DeferredMtrl->SetTexParam(TEX_PARAM::TEX_5, CAssetMgr::GetInst()->FindAsset<CTexture>(L"SpecularTargetTex"));
    }

    // =============
    // IDMap MRT
    // =============
    {
        Ptr<CTexture> arrRTTex[1] = {m_IDMapTex};
        Vec4 arrClear[1] = {Vec4(0.f, 0.f, 0.f, 1.f)};
        Ptr<CTexture> DSTex = m_IDMapDSTex;

        m_arrMRT[(UINT)MRT_TYPE::IDMAP] = new CMRT;
        m_arrMRT[(UINT)MRT_TYPE::IDMAP]->Create(arrRTTex, arrClear, 1, DSTex);
    }

    // =============
    // SSAO MRT
    // =============
    {
        Ptr<CTexture> arrRTTex[1] = {CAssetMgr::GetInst()->CreateTexture(L"SSAOTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                                                         DXGI_FORMAT_R8G8B8A8_UNORM,
                                                                         D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT)};

        Vec4 arrClearColor[1] = {Vec4(1.f, 1.f, 1.f, 1.f)};

        m_arrMRT[(UINT)MRT_TYPE::SSAO] = new CMRT;
        m_arrMRT[(UINT)MRT_TYPE::SSAO]->Create(arrRTTex, arrClearColor, 1, nullptr);

        Ptr<CMaterial> pSSAOMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SSAOMtrl");
        pSSAOMtrl->SetTexParam(TEX_PARAM::TEX_0, m_DepthOnlyTex);
        pSSAOMtrl->SetTexParam(TEX_PARAM::TEX_1, CAssetMgr::GetInst()->FindAsset<CTexture>(L"PositionTargetTex"));
        pSSAOMtrl->SetTexParam(TEX_PARAM::TEX_2, CAssetMgr::GetInst()->FindAsset<CTexture>(L"NormalTargetTex"));
    }
}

void CRenderMgr::CreateIDMapTex(Vec2 Resolution)
{
    m_IDMapTex = CAssetMgr::GetInst()->CreateTexture(L"IDMapTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                     D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT);

    m_IDMapDSTex = CAssetMgr::GetInst()->CreateTexture(L"IDMapDSTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_D24_UNORM_S8_UINT,
                                                       D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);
}

void CRenderMgr::CreateDepthOnlyTex(Vec2 Resolution)
{
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    ZeroMemory(&dsvDesc, sizeof(dsvDesc));
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    m_DepthOnlyTex =
        CAssetMgr::GetInst()->CreateTexture(L"DepthOnlyTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R32_TYPELESS,
                                            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT, &dsvDesc, nullptr, &srvDesc);
}

void CRenderMgr::Resize_Release()
{
    // MRT Texture
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"AlbedoTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"PositionTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"NormalTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"TangentTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"BitangentTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"EmissiveTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"DiffuseTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"SpecularTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"MetallicRoughnessTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"AmbientOcclusionTargetTex");

    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"LightRadianceTargetTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"NormalTargetCopyTex");

    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"SSAOTex");

    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"RTCopyTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"IDMapTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"IDMapDSTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"DepthOnlyTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"PostProessTex_LDRI");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"PostProessTex_HDRI");

    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"LDRI_BloomRenderTexture");
    for (int i = 0; i < m_bloomLevels - 1; i++)
    {
        CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"LDRI_BloomTexture " + std::to_wstring(i));
        CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"HDRI_BloomTexture " + std::to_wstring(i));
        CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"BlurTexture " + std::to_wstring(i));
    }

    Delete_Array(m_arrMRT);
    m_RTCopyTex = nullptr;
    m_DepthOnlyTex = nullptr;
    m_PostProcessTex_LDRI = nullptr;
    m_PostProcessTex_HDRI = nullptr;
    m_FloatRTTex = nullptr;
}

void CRenderMgr::Resize(Vec2 Resolution)
{
    CreateRTCopyTex(Resolution);
    CreateIDMapTex(Resolution);
    CreateDepthOnlyTex(Resolution);
    CreatePostProcessTex(Resolution);
    CreateBloomTextures(Resolution);
    CreateMRT(Resolution);

    m_FloatRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"FloatRenderTargetTexture");

    for (size_t i = 0; i < m_vecCam.size(); i++)
    {
        m_vecCam[i]->Resize(Resolution);
    }

    m_CombineObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_RTCopyTex);
    m_CombineObj->MeshRender()->GetMaterial()->SetTexParam(TEX_1, m_BloomRTTex_LDRI);

    m_ToneMappingObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_FloatRTTex);
    m_ToneMappingObj->MeshRender()->GetMaterial()->SetTexParam(TEX_1, m_PostProcessTex_HDRI);

    m_PostEffectObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_FloatRTTex);
    m_PostEffectObj->MeshRender()->GetMaterial()->SetTexParam(TEX_1, m_DepthOnlyTex);
}
