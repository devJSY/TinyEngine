#include "pch.h"
#include "CHeightMapShader.h"

#include "CConstBuffer.h"
#include "CStructuredBuffer.h"
#include "CTexture.h"

CHeightMapShader::CHeightMapShader()
    : CComputeShader(32, 32, 1)
    , m_pHeightMap(nullptr)
    , m_pBrushTex(nullptr)
    , m_vScale(Vec2())
    , m_Strength(0.f)
    , m_iBrushIdx(0)
    , m_pInput(nullptr)
{
    Create(L"shader\\heightmap.fx", "CS_HeightMap");
}

CHeightMapShader::~CHeightMapShader()
{
}

int CHeightMapShader::UpdateData()
{
    if (nullptr == m_pHeightMap || nullptr == m_pBrushTex)
        return E_FAIL;

    m_Const.arrInt[0] = (int)m_pHeightMap->GetWidth();
    m_Const.arrInt[1] = (int)m_pHeightMap->GetHeight();
    m_Const.arrInt[2] = m_iBrushIdx;
    m_Const.arrFloat[0] = m_Strength;
    m_Const.arrVec2[0] = m_vScale;

    // 높이맵
    m_pHeightMap->UpdateData_CS_UAV(0);

    // 브러쉬
    m_pBrushTex->UpdateData_CS_SRV(0);

    // 피킹정보
    if (nullptr != m_pInput)
    {
        m_pInput->UpdateData_CS_SRV(16);
    }

    return S_OK;
}

void CHeightMapShader::UpdateGroupCount()
{
    // 실행 시킬 스레드 그룹 수 지정
    UINT GroupX = ((UINT)m_pHeightMap->GetWidth() / m_ThreadX) + 1;
    UINT GroupY = ((UINT)m_pHeightMap->GetHeight() / m_ThreadY) + 1;
    UINT GroupZ = 1;

    SetGroup(GroupX, GroupY, GroupZ);
}

void CHeightMapShader::Clear()
{
    if (nullptr != m_pHeightMap)
    {
        m_pHeightMap->Clear_CS_UAV();
        m_pHeightMap = nullptr;
    }

    if (nullptr != m_pBrushTex)
    {
        m_pBrushTex->Clear_CS_SRV();
        m_pBrushTex = nullptr;
    }

    if (nullptr != m_pInput)
    {
        m_pInput->Clear_CS_SRV();
        m_pInput = nullptr;
    }
}