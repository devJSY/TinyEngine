#include "pch.h"
#include "CLight3D.h"
#include "CTransform.h"

#include "CMeshRender.h"
#include "CAssetMgr.h"

CLight3D::CLight3D()
    : CComponent(COMPONENT_TYPE::LIGHT3D)
    , m_Info{}
{
    m_Info.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
    m_Info.vAmbient = Vec4(1.f, 1.f, 1.f, 1.f);
    m_Info.vSpecular = Vec4(1.f, 1.f, 1.f, 1.f);

    m_Info.fRadius = 0.f;
    m_Info.fAngle = 0.f;

    m_Info.LightType = -1;

    m_Info.fallOffStart = 0.f;
    m_Info.fallOffEnd = 1000.f;
    m_Info.spotPower = 100.f;
}

CLight3D::~CLight3D()
{
}

void CLight3D::finaltick()
{
    m_Info.vWorldPos = Transform()->GetWorldPos();
    m_Info.vWorldDir = Transform()->GetLocalDir(DIR_TYPE::FRONT);

    if (LIGHT_TYPE::DIRECTIONAL == (LIGHT_TYPE)m_Info.LightType)
    {
        m_Info.LightType = LIGHT_DIRECTIONAL;
        GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirectionalLight"));
    }
    else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_Info.LightType)
    {
        m_Info.LightType = LIGHT_POINT;
        GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLight"));
    }
    else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_Info.LightType)
    {
        m_Info.LightType = LIGHT_SPOT;
        GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLight"));
    }
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
    fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
    fread(&m_Info, sizeof(tLightInfo), 1, _File);
}
