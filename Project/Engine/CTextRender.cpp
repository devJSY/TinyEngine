#include "pch.h"
#include "CTextRender.h"

#include "CFontMgr.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "components.h"

CTextRender::CTextRender()
    : CRenderComponent(COMPONENT_TYPE::TEXTRENDER)
    , m_Text()
    , m_Size(10.f)
    , m_Color(Vec4(255.f, 255.f, 255.f, 255.f))
    , m_CameraIdx(0)
{
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl")); // Not Use
}

CTextRender::~CTextRender()
{
}

void CTextRender::render()
{
    CCamera* pCam = CRenderMgr::GetInst()->GetCamera(m_CameraIdx);

    if (nullptr == pCam)
        return;

    // World Coordinate → Ndc Coordinate
    Vec3 WorldPos = Transform()->GetWorldPos();
    Matrix viewMat = pCam->GetViewMat();
    Matrix projMat = pCam->GetProjMat();

    Vec4 viewpos = XMVector3Transform(WorldPos, viewMat);
    Vec4 projPos = XMVector4Transform(viewpos, projMat);

    Vec3 NdcPos = Vec3();
    NdcPos.x = projPos.x / projPos.w;
    NdcPos.y = projPos.y / projPos.w;
    NdcPos.z = projPos.z / projPos.w;

    // 화면 밖인경우
    if (NdcPos.x < -1.0 || NdcPos.y < -1.0 || NdcPos.x > 1.0 || NdcPos.y > 1.0)
        return;

    // Ndc Coordinate → Screen Coordinate
    Vec2 Resolution = CDevice::GetInst()->GetRenderResolution();

    float x_screen = (NdcPos.x + 1.0f) * 0.5f * Resolution.x;
    float y_screen = (1.0f - NdcPos.y) * 0.5f * Resolution.y;

    CFontMgr::GetInst()->DrawFont(m_Text.c_str(), x_screen, y_screen, m_Size, FONT_RGBA(m_Color.x, m_Color.y, m_Color.z, m_Color.w));
}

void CTextRender::render(Ptr<CMaterial> _mtrl)
{
    render();
}

void CTextRender::SaveToLevelFile(FILE* _File)
{
    SaveWStringToFile(m_Text, _File);
    fwrite(&m_Size, sizeof(float), 1, _File);
    fwrite(&m_Color, sizeof(Vec4), 1, _File);
    fwrite(&m_CameraIdx, sizeof(int), 1, _File);
}

void CTextRender::LoadFromLevelFile(FILE* _File)
{
    LoadWStringFromFile(m_Text, _File);
    fread(&m_Size, sizeof(float), 1, _File);
    fread(&m_Color, sizeof(Vec4), 1, _File);
    fread(&m_CameraIdx, sizeof(int), 1, _File);
}
