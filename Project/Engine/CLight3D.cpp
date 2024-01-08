#include "pch.h"
#include "CLight3D.h"
#include "CTransform.h"

#include "CMeshRender.h"
#include "CAssetMgr.h"

CLight3D::CLight3D(LIGHT_TYPE type, int idx)
    : CComponent(COMPONENT_TYPE::LIGHT3D)
    , m_Type(type)
    , m_Idx(idx)
{
    m_LightData.fallOffStart = 0.f;
    m_LightData.fallOffEnd = 1000.f;
    m_LightData.spotPower = 100.f;
    m_LightData.color = Vec3(1.f);

    if (m_Type == LIGHT_TYPE::DIRECTIONAL)
        m_LightData.LightType = LIGHT_DIRECTIONAL;
    else if (m_Type == LIGHT_TYPE::POINT)
        m_LightData.LightType = LIGHT_POINT;
    else if (m_Type == LIGHT_TYPE::SPOT)
        m_LightData.LightType = LIGHT_SPOT;
    else
        m_LightData.LightType = LIGHT_OFF;
}

CLight3D::~CLight3D()
{
}

void CLight3D::finaltick()
{
    Matrix Worldmat = GetOwner()->Transform()->GetWorldMat();
    m_LightData.position = Vec3(Worldmat._41, Worldmat._42, Worldmat._43);
    m_LightData.direction = GetOwner()->Transform()->GetLocalDir(DIR_TYPE::FRONT);

    if (m_Type == LIGHT_TYPE::DIRECTIONAL)
    {
        m_LightData.LightType = LIGHT_DIRECTIONAL;
        GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirectionalLight"));
    }
    else if (m_Type == LIGHT_TYPE::POINT)
    {
        m_LightData.LightType = LIGHT_POINT;
        GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLight"));
    }
    else if (m_Type == LIGHT_TYPE::SPOT)
    {
        m_LightData.LightType = LIGHT_SPOT;
        GetOwner()->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLight"));
    }
    else
        m_LightData.LightType = LIGHT_OFF;

    g_Global.Lights[m_Idx] = m_LightData;
}

void CLight3D::SaveToLevelFile(FILE* _File)
{
}

void CLight3D::LoadFromLevelFile(FILE* _File)
{
}
