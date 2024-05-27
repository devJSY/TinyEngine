#include "pch.h"
#include "CWeightMapShader.h"

CWeightMapShader::CWeightMapShader()
    : CComputeShader(32, 32, 1)
    , m_WeightMap(nullptr)
    , m_RaycastInfo(nullptr)
    , m_BrushIdx(0)
    , m_WeightIdx(0)
    , m_Width(0)
    , m_Height(0)
{
    Create(L"shader\\weightmap.fx", "CS_WeightMap");
}

CWeightMapShader::~CWeightMapShader()
{
}

int CWeightMapShader::UpdateData()
{
    if (!m_WeightMap || !m_BrushArrTex.Get() || !m_RaycastInfo)
        return E_FAIL;

    m_Const.arrInt[0] = (int)m_Width;
    m_Const.arrInt[1] = (int)m_Height;
    m_Const.arrInt[2] = m_BrushIdx;
    m_Const.arrInt[3] = m_WeightIdx;
    m_Const.arrVec2[0] = m_BrushScale;

    m_WeightMap->UpdateData_CS_UAV(0);
    m_BrushArrTex->UpdateData_CS_SRV(0);
    m_RaycastInfo->UpdateData_CS_SRV(16);

    return S_OK;
}

void CWeightMapShader::UpdateGroupCount()
{
    // 그룹 수 계산
    UINT GroupX = (m_Width / m_ThreadX) + 1;
    UINT GroupY = (m_Height / m_ThreadY) + 1;
    UINT GroupZ = 1;

    SetGroup(GroupX, GroupY, GroupZ);
}

void CWeightMapShader::Clear()
{
    if (nullptr != m_WeightMap)
    {
        m_WeightMap->Clear_CS_UAV();
        m_WeightMap = nullptr;
    }

    if (nullptr != m_BrushArrTex)
    {
        m_BrushArrTex->Clear_CS_SRV();
        m_BrushArrTex = nullptr;
    }

    if (nullptr != m_RaycastInfo)
    {
        m_RaycastInfo->Clear_CS_SRV();
        m_RaycastInfo = nullptr;
    }
}
