#include "pch.h"
#include "CCamera.h"

#include "CAssetMgr.h"
#include "CLevelMgr.h"

#include "CDevice.h"
#include "CTransform.h"

#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CMeshRender.h"
#include "CMaterial.h"

#include "CRenderComponent.h"

CCamera::CCamera()
    : CComponent(COMPONENT_TYPE::CAMERA)
    , m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
    , m_FOV(XM_PI / 2.f)
    , m_Width(0.f)
    , m_Scale(1.f)
    , m_AspectRatio(1.f)
    , m_Near(1.f)
    , m_Far(10000.f)
    , m_CamSpeed(250.f)
    , m_LayerCheck(0)
    , m_iCamPriority(-1)
{
    Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
    m_AspectRatio = vResol.x / vResol.y;
}

CCamera::~CCamera()
{
}

void CCamera::begin()
{
    if (-1 != m_iCamPriority)
    {
        CRenderMgr::GetInst()->RegisterCamera(this, m_iCamPriority);
    }
}

void CCamera::finaltick()
{
    // 매프레임 화면비 계산
    Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
    if (!(vResol.x <= 0.f || vResol.y <= 0.f)) // 창 최소화 예외처리
    {
        m_Width = vResol.x;
        m_AspectRatio = vResol.x / vResol.y;
    }

    // 카메라 속도 제한
    if (m_CamSpeed < 0.f)
        m_CamSpeed = 0.f;

    // =====================
    // 뷰 행렬 계산
    // =====================
    // 카메라를 원점으로 이동시키는 이동 행렬
    Vec3 vCamPos = Transform()->GetRelativePos();
    Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

    //// 카메라의 각 우, 상, 전 방 방향을 기저축이랑 일치시키도록 회전하는 회전행렬
    Vec3 vCamRot = Transform()->GetRelativeRotation();
    Matrix matRotate =
        Matrix::CreateRotationX(vCamRot.x) * Matrix::CreateRotationY(vCamRot.y) * Matrix::CreateRotationZ(vCamRot.z);
    matRotate = matRotate.Transpose(); // 직교행렬의 역행렬은 전치행렬

    // 이동 x 회전 = view 행렬
    m_matView = matTrans * matRotate;

    // ===============================
    // 투영 방식에 따른 투영 행렬 계산
    // ===============================
    m_matProj = XMMatrixIdentity();

    if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
    {
        // 최소 스케일 제한
        if (m_Scale <= 0.f)
            m_Scale = 1e-5f;

        // 직교투영
        m_matProj = XMMatrixOrthographicLH(m_Width * m_Scale, (m_Width / m_AspectRatio) * m_Scale, m_Near, m_Far);
    }
    else
    {
        // FOV 제한
        float offset = XM_1DIV2PI; // 1/2π 1도 제한
        if (m_FOV < XM_1DIV2PI)
            m_FOV = offset;
        else if (m_FOV > XM_PI - offset)
            m_FOV = XM_PI - offset;

        // 원근투영
        m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, m_Near, m_Far);
    }
}

void CCamera::SetCameraPriority(int _Priority)
{
    m_iCamPriority = _Priority;
    CRenderMgr::GetInst()->RegisterCamera(this, m_iCamPriority);
}

void CCamera::LayerCheck(UINT _LayerIdx, bool _bCheck)
{
    if (_bCheck)
    {
        m_LayerCheck |= (1 << _LayerIdx);
    }
    else
    {
        m_LayerCheck &= ~(1 << _LayerIdx);
    }
}

void CCamera::LayerCheck(const wstring& _strLayerName, bool _bCheck)
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    CLayer* pLayer = pCurLevel->GetLayer(_strLayerName);

    if (nullptr == pLayer)
        return;

    int idx = pLayer->GetLayerIdx();
    LayerCheck(idx, _bCheck);
}

void CCamera::SortObject()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    for (int i = 0; i < LAYER_MAX; ++i)
    {
        // 카메라가 찍도록 설정된 Layer 가 아니면 무시
        if (false == (m_LayerCheck & (1 << i)))
            continue;

        CLayer* pLayer = pCurLevel->GetLayer(i);
        const vector<CGameObject*>& vecObjects = pLayer->GetLayerObjects();
        for (size_t j = 0; j < vecObjects.size(); ++j)
        {
            // 메쉬, 재질, 쉐이더 확인
            if (!(vecObjects[j]->GetRenderComopnent() && vecObjects[j]->GetRenderComopnent()->GetMesh().Get() &&
                  vecObjects[j]->GetRenderComopnent()->GetMaterial().Get() &&
                  vecObjects[j]->GetRenderComopnent()->GetMaterial()->GetShader().Get()))
            {
                continue;
            }

            SHADER_DOMAIN domain = vecObjects[j]->GetRenderComopnent()->GetMaterial()->GetShader()->GetDomain();

            switch (domain)
            {
            case SHADER_DOMAIN::DOMAIN_OPAQUE:
                m_vecOpaque.push_back(vecObjects[j]);
                break;
            case SHADER_DOMAIN::DOMAIN_MASKED:
                m_vecMaked.push_back(vecObjects[j]);
                break;
            case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
                m_vecTransparent.push_back(vecObjects[j]);
                break;
            case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
                m_vecPostProcess.push_back(vecObjects[j]);
                break;
            case SHADER_DOMAIN::DOMAIN_DEBUG:
                break;
            }
        }
    }
}

void CCamera::render()
{
    // 계산한 view 행렬과 proj 행렬을 전역변수에 담아둔다.
    g_Transform.matView = m_matView;
    g_Transform.matProj = m_matProj;

    // eyePos 등록
    g_Global.eyeWorld = Transform()->GetWorldPos();

    // Domain 순서대로 렌더링
    render(m_vecOpaque);
    render(m_vecMaked);
    render(m_vecTransparent);
    render(m_vecPostProcess);
}

void CCamera::render(vector<CGameObject*>& _vecObj)
{
    for (size_t i = 0; i < _vecObj.size(); ++i)
    {
        wstring LayerName = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(_vecObj[i]->GetLayerIdx())->GetName();

        // Render Pass
        _vecObj[i]->render();

        CMeshRender* meshRender = _vecObj[i]->MeshRender();
        if (nullptr != meshRender)
        {
            Ptr<CMaterial> mtrl = meshRender->GetMaterial();
            if (nullptr != mtrl)
            {
                Ptr<CGraphicsShader> shader = mtrl->GetShader();

                // Normal Line Pass
                if (meshRender->IsDrawNormalLine())
                {
                    mtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"NormalLine"));
                    _vecObj[i]->render();
                }

                // outline pass
                // 와이어 프레임일때는 Off
                // SkyBox 일때는 Off
                if (CLevelMgr::GetInst()->GetSelectedObject() == _vecObj[i] && !g_Global.DrawAsWireFrame &&
                    LayerName != L"SkyBox")
                {
                    mtrl->SetShader(CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"OutLine"));
                    _vecObj[i]->render();
                }

                // IDMap
                if (LayerName != L"UI" && LayerName != L"Light" && LayerName != L"Camera")
                {
                    Ptr<CTexture> pIDMapTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"IDMapTex");
                    Ptr<CTexture> pIDMapDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"IDMapDSTex");

                    CONTEXT->OMSetRenderTargets(1, pIDMapTex->GetRTV().GetAddressOf(), pIDMapDSTex->GetDSV().Get());

                    Ptr<CGraphicsShader> IDShader = CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"IDMap");

                    if (LayerName == L"SkyBox")
                        IDShader = CAssetMgr::GetInst()->FindAsset<CGraphicsShader>(L"SkyBox_IDMap");

                    mtrl->SetShader(IDShader);
                    _vecObj[i]->render();

                    Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
                    Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

                    CONTEXT->OMSetRenderTargets(1, pTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());
                }

                // 원래 쉐이더로 설정
                mtrl->SetShader(shader);
            }
        }
    }

    _vecObj.clear();
}

void CCamera::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
    fwrite(&m_FOV, sizeof(float), 1, _File);
    fwrite(&m_Width, sizeof(float), 1, _File);
    fwrite(&m_Scale, sizeof(float), 1, _File);
    fwrite(&m_AspectRatio, sizeof(float), 1, _File);
    fwrite(&m_Near, sizeof(float), 1, _File);
    fwrite(&m_Far, sizeof(float), 1, _File);
    fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
    fwrite(&m_CamSpeed, sizeof(float), 1, _File);
    fwrite(&m_iCamPriority, sizeof(int), 1, _File);
}

void CCamera::LoadFromLevelFile(FILE* _File)
{
    fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
    fread(&m_FOV, sizeof(float), 1, _File);
    fread(&m_Width, sizeof(float), 1, _File);
    fread(&m_Scale, sizeof(float), 1, _File);
    fread(&m_AspectRatio, sizeof(float), 1, _File);
    fread(&m_Near, sizeof(float), 1, _File);
    fread(&m_Far, sizeof(float), 1, _File);
    fread(&m_LayerCheck, sizeof(UINT), 1, _File);
    fread(&m_CamSpeed, sizeof(float), 1, _File);
    fread(&m_iCamPriority, sizeof(int), 1, _File);
}
