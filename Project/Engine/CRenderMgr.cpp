#include "pch.h"
#include "CRenderMgr.h"

#include "CStructuredBuffer.h"

#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "components.h"

#include "CConstBuffer.h"

CRenderMgr::CRenderMgr()
    : m_Light2DBuffer(nullptr)
    , m_Light3DBuffer(nullptr)
    , m_pDebugObj(nullptr)
    , m_bShowDebugRender(false)
    , m_bShowCollider(true)
    , m_vecNoiseTex{}
    , m_ToneMappingObj(nullptr)
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

    if (nullptr != m_ToneMappingObj)
    {
        delete m_ToneMappingObj;
        m_ToneMappingObj = nullptr;
    }
}

void CRenderMgr::tick()
{
    UpdateData();

    render();

    render_debug();

    // ToneMapping
    CDevice::GetInst()->SetRenderTarget();
    m_ToneMappingObj->render();

    Clear();
}

void CRenderMgr::render()
{
    for (size_t i = 0; i < m_vecCam.size(); ++i)
    {
        m_vecCam[i]->SortObject();
        m_vecCam[i]->render();
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
        case DEBUG_SHAPE::CUBE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CubeMesh_Debug"));
            break;
        case DEBUG_SHAPE::SPHERE:
            m_pDebugObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh_Debug"));
            break;
        default:
            break;
        }

        m_pDebugObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
        m_pDebugObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

        D3D11_PRIMITIVE_TOPOLOGY PrevTopology = m_pDebugObj->MeshRender()->GetMaterial()->GetShader()->GetTopology();
        if (DEBUG_SHAPE::CROSS == (*iter).eShape)
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

void CRenderMgr::UpdateData()
{
    // GlobalData 에 광원 개수정보 세팅
    g_Global.Light2DCount = (UINT)m_vecLight2D.size();
    g_Global.Light3DCount = (UINT)m_vecLight3D.size();

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
    m_vecLight2D.clear();
    m_vecLight3D.clear();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _Idx)
{
    if (m_vecCam.size() <= _Idx + 1)
    {
        m_vecCam.resize(_Idx + 1);
    }

    m_vecCam[_Idx] = _Cam;
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

void CRenderMgr::CopyToPostProcessTex()
{
    CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), m_ResolvedFloatTex->GetTex2D().Get());
}

void CRenderMgr::CreateRTCopyTex(Vec2 Resolution)
{
    m_RTCopyTex = CAssetMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                                      DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE,
                                                      D3D11_USAGE_DEFAULT);
}

void CRenderMgr::CreatePostProcessTex(Vec2 Resolution)
{
    m_PostProcessTex = CAssetMgr::GetInst()->CreateTexture(L"PostProessTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                                           DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_SHADER_RESOURCE,
                                                           D3D11_USAGE_DEFAULT);
}

void CRenderMgr::CreateIDMapTex(Vec2 Resolution)
{
    m_IDMapTex = CAssetMgr::GetInst()->CreateTexture(
        L"IDMapTex", (UINT)Resolution.x, (UINT)Resolution.y, DXGI_FORMAT_R8G8B8A8_UNORM,
        D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET, D3D11_USAGE_DEFAULT);

    m_IDMapDSTex = CAssetMgr::GetInst()->CreateTexture(L"IDMapDSTex", (UINT)Resolution.x, (UINT)Resolution.y,
                                                       DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL,
                                                       D3D11_USAGE_DEFAULT);
}

void CRenderMgr::Resize(Vec2 Resolution)
{
    m_RTCopyTex = nullptr;
    m_IDMapTex = nullptr;
    m_IDMapDSTex = nullptr;
    m_PostProcessTex = nullptr;

    CreateRTCopyTex(Resolution);
    CreateIDMapTex(Resolution);
    CreatePostProcessTex(Resolution);

    m_FloatTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"FloatTexture");
    m_ResolvedFloatTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"ResolvedFloatTexture");
}

void CRenderMgr::ResolveFloatTexture()
{
    CONTEXT->ResolveSubresource(m_ResolvedFloatTex->GetTex2D().Get(), 0, m_FloatTex->GetTex2D().Get(), 0,
                                DXGI_FORMAT_R16G16B16A16_FLOAT);
}
