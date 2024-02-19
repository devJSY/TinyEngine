#include "pch.h"
#include "CCamera.h"

#include "CAssetMgr.h"
#include "CEditorMgr.h"

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
#include "CConstBuffer.h"

CCamera::CCamera()
    : CComponent(COMPONENT_TYPE::CAMERA)
    , m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
    , m_FOV(XM_PI / 2.f)
    , m_Width(0.f)
    , m_Scale(1.f)
    , m_AspectRatio(1.f)
    , m_Near(1.f)
    , m_Far(10000.f)
    , m_LayerCheck(0)
    , m_iCamPriority(-1)
{
    Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
    m_Width = vResol.x;
    m_AspectRatio = vResol.x / vResol.y;
}

CCamera::~CCamera()
{
}

void CCamera::begin()
{
    if (-1 == m_iCamPriority)
    {
        CRenderMgr::GetInst()->RegisterUICamera(this);
    }
    else
    {
        CRenderMgr::GetInst()->RegisterCamera(this, m_iCamPriority);
    }
}

void CCamera::finaltick()
{
    // =====================
    // 뷰 행렬 계산
    // =====================
    // 카메라를 원점으로 이동시키는 이동 행렬
    Vec3 vCamPos = Transform()->GetRelativePos();
    Matrix matTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);

    //// 카메라의 각 우, 상, 전 방 방향을 기저축이랑 일치시키도록 회전하는 회전행렬
    Vec3 vCamRot = Transform()->GetRelativeRotation();
    Matrix matRotate = Matrix::CreateRotationX(vCamRot.x) * Matrix::CreateRotationY(vCamRot.y) * Matrix::CreateRotationZ(vCamRot.z);
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

void CCamera::LayerCheck(CLevel* _CurLevel, const wstring& _strLayerName, bool _bCheck)
{
    CLayer* pLayer = _CurLevel->GetLayer(_strLayerName);

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
            if (!(vecObjects[j]->GetRenderComponent() && vecObjects[j]->GetRenderComponent()->GetMesh().Get() &&
                  vecObjects[j]->GetRenderComponent()->GetMaterial().Get() && vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader().Get()))
            {
                continue;
            }

            SHADER_DOMAIN domain = vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader()->GetDomain();

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
            case SHADER_DOMAIN::DOMAIN_MIRROR: {
                if (0 == g_Global.render_Mode)
                {
                    CRenderMgr::GetInst()->RegisterMirror(vecObjects[j]); // Mirror 는 RenderMgr 에서 관리
                }
            }
            break;
            case SHADER_DOMAIN::DOMAIN_POSTPROCESS: {
                if (0 == g_Global.render_Mode)
                {
                    CRenderMgr::GetInst()->RegisterPostProcess(vecObjects[j]); // 후처리는 RenderMgr 에서 관리
                }
            }
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
    if (2 == g_Global.render_Mode)
        g_Transform.matView = g_Global.ReflectionRowMat * g_Transform.matView;
    g_Transform.matViewInv = m_matView.Invert();
    g_Transform.matProj = m_matProj;
    g_Transform.matProjInv = m_matProj.Invert();

    // Domain 순서대로 렌더링
    render(m_vecOpaque);
    render(m_vecMaked);
    render(m_vecTransparent);
}

void CCamera::clear()
{
    m_vecOpaque.clear();
    m_vecMaked.clear();
    m_vecTransparent.clear();
}

void CCamera::render_DepthMap()
{
    render_DepthMap(m_vecOpaque);
    render_DepthMap(m_vecMaked);
    render_DepthMap(m_vecTransparent);
}

void CCamera::render_NormalLine()
{
    render_NormalLine(m_vecOpaque);
    render_NormalLine(m_vecMaked);
    render_NormalLine(m_vecTransparent);
}

void CCamera::render_IDMap()
{
    render_IDMap(m_vecOpaque);
    render_IDMap(m_vecMaked);
    render_IDMap(m_vecTransparent);
}

void CCamera::render(vector<CGameObject*>& _vecObj)
{
    for (size_t i = 0; i < _vecObj.size(); ++i)
    {
        // Render Pass
        if (g_Global.DrawAsWireFrame)
        {
            RS_TYPE originRSType = _vecObj[i]->GetRenderComponent()->GetMaterial()->GetShader()->GetRSType();
            _vecObj[i]->GetRenderComponent()->GetMaterial()->GetShader()->SetRSType(RS_TYPE::WIRE_FRAME);
            _vecObj[i]->render();
            _vecObj[i]->GetRenderComponent()->GetMaterial()->GetShader()->SetRSType(originRSType);
        }
        else
        {
            _vecObj[i]->render();
        }
    }
}

void CCamera::render_DepthMap(vector<CGameObject*>& _vecObj)
{
    for (size_t i = 0; i < _vecObj.size(); i++)
    {
        _vecObj[i]->render(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DepthOnlyMtrl"));
    }
}

void CCamera::render_NormalLine(vector<CGameObject*>& _vecObj)
{
    for (size_t i = 0; i < _vecObj.size(); i++)
    {
        _vecObj[i]->render(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"NormalLineMtrl"));
    }
}

void CCamera::render_IDMap(vector<CGameObject*>& _vecObj)
{
    for (size_t i = 0; i < _vecObj.size(); i++)
    {
        wstring LayerName = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(_vecObj[i]->GetLayerIdx())->GetName();
        // 특정 레이어는 IDMap 에서 제외
        if (LayerName != L"UI" && LayerName != L"Camera" && LayerName != L"SkyBox")
        {
            // 오브젝트 이름 + ID값으로 HashID Find
            hash<wstring> hasher;
            int HashID = (int)hasher(_vecObj[i]->GetName()) + _vecObj[i]->GetID();

            Ptr<CMaterial> IDMapMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"IDMapMtrl");
            IDMapMtrl->SetScalarParam(VEC4_0, HashIDToColor(HashID));
            _vecObj[i]->render(IDMapMtrl);
        }
    }
}

void CCamera::Resize(Vec2 Resolution)
{
    if (Resolution.x <= 0.f || Resolution.y <= 0.f) // 창 최소화 예외처리
        return;

    m_Width = Resolution.x;
    m_AspectRatio = Resolution.x / Resolution.y;
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
    fread(&m_iCamPriority, sizeof(int), 1, _File);
}
