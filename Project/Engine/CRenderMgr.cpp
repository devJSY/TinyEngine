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

CRenderMgr::CRenderMgr()
    : m_CamUI(nullptr)
    , m_Light2DBuffer(nullptr)
    , m_Light3DBuffer(nullptr)
    , m_pDebugObj(nullptr)
    , m_bShowDebugRender(false)
    , m_bShowCollider(true)
    , m_vecNoiseTex{}
    , m_DepthOnlyTex{}
    , m_PostEffectObj(nullptr)
    , m_vecPostProcess{}
    , bloomLevels(5)
    , m_BloomTextures{}
    , m_BloomDownFilters{}
    , m_BloomUpFilters{}
    , m_ToneMappingObj(nullptr)
    , m_Mirror(nullptr)
{
}

CRenderMgr::~CRenderMgr()
{
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

    Delete_Vec(m_BloomDownFilters);
    Delete_Vec(m_BloomUpFilters);

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

void CRenderMgr::tick()
{
    UpdateData();

    // Object 정렬
    for (size_t i = 0; i < m_vecCam.size(); ++i)
        m_vecCam[i]->SortObject();

    // Light Depth Map
    render_LightDepth();

    // HDR Rendering
    render();

    // Mirror
    render_mirror();

    // Debug
    render_debug();

    // PostEffect
    render_posteffect();

    // Postprocess
    render_postprocess();

    // LDR Rendering
    render_ui();

    Clear();
}

void CRenderMgr::render()
{
    CDevice::GetInst()->SetFloatRenderTarget();

    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        // Main Render
        m_vecCam[i]->render();

        // Depth Map Pass
        CONTEXT->OMSetRenderTargets(0, NULL, m_DepthOnlyTex->GetDSV().Get());
        m_vecCam[i]->render_DepthMap();
        CDevice::GetInst()->SetFloatRenderTarget();

        // NormalLine Pass
        m_vecCam[i]->render_NormalLine();

        // OutLine Pass
        CGameObject* pSelectedObj = CEditorMgr::GetInst()->GetSelectedObject();
        if (nullptr != pSelectedObj && pSelectedObj != m_Mirror && !g_Global.DrawAsWireFrame)
        {
            if (PROJ_TYPE::ORTHOGRAPHIC == m_vecCam[i]->GetProjType())
                pSelectedObj->render(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"2D_OutLineMtrl"));
            else
                pSelectedObj->render(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"3D_OutLineMtrl"));
        }

        // IDMap Pass
        CONTEXT->OMSetRenderTargets(1, m_IDMapTex->GetRTV().GetAddressOf(), m_IDMapDSTex->GetDSV().Get());
        m_vecCam[i]->render_IDMap();
        CDevice::GetInst()->SetFloatRenderTarget();
    }
}

void CRenderMgr::render_debug()
{
    g_Transform.matView = m_vecCam[0]->GetViewMat();
    g_Transform.matProj = m_vecCam[0]->GetProjMat();

    list<tDebugShapeInfo>::iterator iter = m_DbgShapeInfo.begin();
    for (; iter != m_DbgShapeInfo.end();)
    {
        switch ((*iter).eShape)
        {
        case DEBUG_SHAPE::RECT:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
            break;
        case DEBUG_SHAPE::CIRCLE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
            break;
        case DEBUG_SHAPE::CROSS:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CrosshairMesh"));
            break;
        case DEBUG_SHAPE::BOX:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"WireBox"));
            break;
        case DEBUG_SHAPE::SPHERE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"WireSphere"));
            break;
        default:
            break;
        }

        m_pDebugObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
        m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

        D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->GetTopology();
        if (DEBUG_SHAPE::CROSS == (*iter).eShape || DEBUG_SHAPE::BOX == (*iter).eShape || DEBUG_SHAPE::SPHERE == (*iter).eShape)
        {
            m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        }

        m_pDebugObj->Transform()->SetWorldMat((*iter).matWorld);
        m_pDebugObj->Transform()->UpdateData();

        m_pDebugObj->render();

        m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->SetTopology(PrevTopology);

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

void CRenderMgr::render_mirror()
{
    if (nullptr == m_Mirror)
        return;

    g_Transform.matView = m_vecCam[0]->GetViewMat();
    g_Transform.matProj = m_vecCam[0]->GetProjMat();

    // 거울부분 masking
    CDevice::GetInst()->ClearStencil();
    g_Global.render_Mode = 1; //  Stencil Mask
    m_Mirror->render();

    // masking 부분 렌더
    CDevice::GetInst()->ClearDepth();
    g_Global.ReflectionRowMat =
        Matrix::CreateReflection(SimpleMath::Plane(m_Mirror->Transform()->GetWorldPos(), m_Mirror->Transform()->GetWorldDir(DIR_TYPE::FRONT)));
    g_Global.render_Mode = 2; // Masked Render
    render();

    // 거울 렌더링
    g_Global.render_Mode = 0; // Render
    m_Mirror->render();

    // Mirror DepthOnlyPass
    CONTEXT->OMSetRenderTargets(1, m_PostProcessTex->GetRTV().GetAddressOf(), m_DepthOnlyTex->GetDSV().Get());
    m_Mirror->render(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DepthOnlyMtrl"));
    CDevice::GetInst()->SetFloatRenderTarget();

    // Reset
    m_Mirror = nullptr;
}

void CRenderMgr::render_posteffect()
{
    if (nullptr == m_PostEffectObj)
        return;

    // RTV(PostProcess) , SRV(floatRTTex DepthOnlyTex)
    CONTEXT->OMSetRenderTargets(1, m_PostProcessTex->GetRTV().GetAddressOf(), NULL);
    m_PostEffectObj->render();
    CTexture::Clear(0);
    CTexture::Clear(1);
    CDevice::GetInst()->SetFloatRenderTarget();
    CONTEXT->CopyResource(m_FloatRTTex->GetTex2D().Get(), m_PostProcessTex->GetTex2D().Get());
}

void CRenderMgr::render_ui()
{
    if (nullptr == m_CamUI)
        return;

    CDevice::GetInst()->SetRenderTarget();

    m_CamUI->SortObject();
    m_CamUI->render();
}

void CRenderMgr::render_LightDepth()
{
    tTransform originTr = g_Transform;

    // 그림자 적용 광원 최대갯수설정
    int dynamicShadowMaxCount = 3;

    for (int i = 0; i < m_vecLight3D.size(); i++)
    {
        m_vecLight3D[i]->SetShadowIdx(-1); // 초기화

        if (dynamicShadowMaxCount <= 0)
            break;

        const tLightInfo& info = m_vecLight3D[i]->GetLightInfo();
        if (!info.ShadowType)
            continue;

        // 광원 시점 렌더링
        g_Transform.matView = info.viewMat;
        g_Transform.matViewInv = g_Transform.matView.Invert();
        g_Transform.matProj = info.projMat;
        g_Transform.matProjInv = g_Transform.matProj.Invert();

        m_vecLight3D[i]->SetShadowIdx(3 - dynamicShadowMaxCount);
        Ptr<CTexture> DepthMapTex = m_vecLight3D[i]->GetDepthMapTex();

        CONTEXT->ClearDepthStencilView(DepthMapTex->GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
        CONTEXT->OMSetRenderTargets(0, NULL, DepthMapTex->GetDSV().Get());
        CDevice::GetInst()->SetViewport((float)DepthMapTex->GetWidth(), (float)DepthMapTex->GetHeight());

        for (size_t i = 0; i < m_vecCam.size(); ++i)
        {
            m_vecCam[i]->render_DepthMap();
        }

        CDevice::GetInst()->SetFloatRenderTarget();
        CDevice::GetInst()->SetViewport();

        if (3 == dynamicShadowMaxCount)
            DepthMapTex->UpdateData(21);
        else if (2 == dynamicShadowMaxCount)
            DepthMapTex->UpdateData(22);
        else if (1 == dynamicShadowMaxCount)
            DepthMapTex->UpdateData(23);

        dynamicShadowMaxCount--;
    }

    g_Transform = originTr;
}

void CRenderMgr::render_postprocess()
{
    // 후처리
    for (int i = 0; i < m_vecPostProcess.size(); ++i)
    {
        // 최종 렌더링 이미지를 후처리 타겟에 복사
        CopyToPostProcessTex();

        // 복사받은 후처리 텍스쳐를 t14 레지스터에 바인딩
        m_PostProcessTex->UpdateData(14);

        // 후처리 오브젝트 렌더링
        m_vecPostProcess[i]->render();
    }

    m_vecPostProcess.clear();
    CTexture::Clear(14);

    // Bloom
    CopyToPostProcessTex();
    for (int i = 0; i < m_BloomDownFilters.size(); i++)
    {
        if (i == 0)
            m_BloomDownFilters[i]->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_PostProcessTex);
        else
            m_BloomDownFilters[i]->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_BloomTextures[i - 1]);

        CDevice::GetInst()->SetViewport((float)m_BloomTextures[i]->GetWidth(), (float)m_BloomTextures[i]->GetHeight());
        CONTEXT->OMSetRenderTargets(1, m_BloomTextures[i]->GetRTV().GetAddressOf(), NULL);
        m_BloomDownFilters[i]->render();
        CTexture::Clear(0);
    }
    for (int i = 0; i < m_BloomUpFilters.size(); i++)
    {
        int level = bloomLevels - 2 - i;
        m_BloomUpFilters[i]->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_BloomTextures[level]);
        if (i == bloomLevels - 2)
        {
            CDevice::GetInst()->SetViewport((float)m_PostProcessTex->GetWidth(), (float)m_PostProcessTex->GetHeight());
            CONTEXT->OMSetRenderTargets(1, m_PostProcessTex->GetRTV().GetAddressOf(), NULL);
        }
        else
        {
            CDevice::GetInst()->SetViewport((float)m_BloomTextures[level - 1]->GetWidth(), (float)m_BloomTextures[level - 1]->GetHeight());
            CONTEXT->OMSetRenderTargets(1, m_BloomTextures[level - 1]->GetRTV().GetAddressOf(), NULL);
        }
        m_BloomUpFilters[i]->render();
        CTexture::Clear(0);
    }

    // Tone Mapping + Bloom Combine
    CDevice::GetInst()->SetViewport();
    CDevice::GetInst()->SetRenderTarget();
    m_ToneMappingObj->render();
    CTexture::Clear(0);
    CTexture::Clear(1);
}

void CRenderMgr::UpdateData()
{
    // GlobalData 에 광원 개수정보 세팅
    g_Global.Light2DCount = (UINT)m_vecLight2D.size();
    g_Global.Light3DCount = (UINT)m_vecLight3D.size();

    // 메인 카메라 위치 등록
    g_Global.eyeWorld = m_vecCam[0]->Transform()->GetWorldPos();

    // 전역 상수 데이터 바인딩
    CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL_DATA);
    pGlobalBuffer->SetData(&g_Global);
    pGlobalBuffer->UpdateData();
    pGlobalBuffer->UpdateData_CS();

    // Light2D
    static vector<tLightInfo> vecLight2DInfo;

    for (size_t i = 0; i < m_vecLight2D.size(); ++i)
    {
        const tLightInfo& info = m_vecLight2D[i]->GetLightInfo();
        vecLight2DInfo.push_back(info);
    }

    m_Light2DBuffer->SetData(vecLight2DInfo.data(), (UINT)vecLight2DInfo.size());
    m_Light2DBuffer->UpdateData(11);

    vecLight2DInfo.clear();

    // Light3D
    static vector<tLightInfo> vecLight3DInfo;

    for (size_t i = 0; i < m_vecLight3D.size(); ++i)
    {
        const tLightInfo& info = m_vecLight3D[i]->GetLightInfo();
        vecLight3DInfo.push_back(info);
    }

    m_Light3DBuffer->SetData(vecLight3DInfo.data(), (UINT)vecLight3DInfo.size());
    m_Light3DBuffer->UpdateData(12);

    vecLight3DInfo.clear();
}

void CRenderMgr::Clear()
{
    for (int i = 0; i < m_vecCam.size(); i++)
    {
        m_vecCam[i]->clear();
    }

    if (nullptr != m_CamUI)
        m_CamUI->clear();

    m_vecLight2D.clear();
    m_vecLight3D.clear();

    // Light DepthMap bind Reset
    CTexture::Clear(21);
    CTexture::Clear(22);
    CTexture::Clear(23);
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Idx)
{
    if (m_vecCam.size() <= _Idx + 1)
    {
        m_vecCam.resize(_Idx + 1);
    }

    m_vecCam[_Idx] = _Cam;
}

void CRenderMgr::RegisterUICamera(CCamera* _Cam)
{
    m_CamUI = _Cam;
}

CCamera* CRenderMgr::GetCamera(int _Idx) const
{
    if (m_vecCam.size() <= _Idx)
        return nullptr;

    return m_vecCam[_Idx];
}

void CRenderMgr::Clear_Buffers(const Vec4& Color)
{
    CONTEXT->ClearRenderTargetView(m_IDMapTex->GetRTV().Get(), Color);
    CONTEXT->ClearDepthStencilView(m_IDMapDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    CONTEXT->ClearRenderTargetView(m_PostProcessTex->GetRTV().Get(), Color);

    CONTEXT->ClearDepthStencilView(m_DepthOnlyTex->GetDSV().Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CRenderMgr::CopyRTTexToRTCopyTex()
{
    Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
    CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::CopyToPostProcessTex()
{
    CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), m_FloatRTTex->GetTex2D().Get());
}

void CRenderMgr::CreateRTCopyTex(Vec2 Resolution)
{
    m_RTCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
                                                      D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
}

void CRenderMgr::CreatePostProcessTex(Vec2 Resolution)
{
    m_PostProcessTex = CAssetMgr::GetInst()->CreateTexture(L"PostProessTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R16G16B16A16_FLOAT,
                                                           D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
}

void CRenderMgr::CreateBloomTextures(Vec2 Resolution)
{
    m_BloomTextures.clear();

    for (int i = 0; i < bloomLevels - 1; i++)
    {
        int div = int(pow(2, 1 + i));
        m_BloomTextures.push_back(CAssetMgr::GetInst()->CreateTexture(L"BloomTexture " + std::to_wstring(i), UINT(Resolution.x / div),
                                                                      UINT(Resolution.y / div), DXGI_FORMAT_R16G16B16A16_FLOAT,
                                                                      D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT));
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

void CRenderMgr::Resize(Vec2 Resolution)
{
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"RTCopyTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"IDMapTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"IDMapDSTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"DepthOnlyTex");
    CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"PostProessTex");

    for (int i = 0; i < bloomLevels - 1; i++)
    {
        CAssetMgr::GetInst()->DeleteAsset(ASSET_TYPE::TEXTURE, L"BloomTexture " + std::to_wstring(i));
    }

    m_RTCopyTex = nullptr;
    m_IDMapTex = nullptr;
    m_IDMapDSTex = nullptr;
    m_DepthOnlyTex = nullptr;
    m_PostProcessTex = nullptr;
    m_FloatRTTex = nullptr;

    CreateRTCopyTex(Resolution);
    CreateIDMapTex(Resolution);
    CreateDepthOnlyTex(Resolution);
    CreatePostProcessTex(Resolution);
    CreateBloomTextures(Resolution);

    m_FloatRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"FloatRenderTargetTexture");

    for (size_t i = 0; i < m_vecCam.size(); i++)
    {
        m_vecCam[i]->Resize(Resolution);
    }

    if (nullptr != m_CamUI)
    {
        m_CamUI->Resize(Resolution);
    }

    m_ToneMappingObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_FloatRTTex);
    m_ToneMappingObj->MeshRender()->GetMaterial()->SetTexParam(TEX_1, m_PostProcessTex);

    m_PostEffectObj->MeshRender()->GetMaterial()->SetTexParam(TEX_0, m_FloatRTTex);
    m_PostEffectObj->MeshRender()->GetMaterial()->SetTexParam(TEX_1, m_DepthOnlyTex);
}
