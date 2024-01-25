#include "pch.h"
#include "CSetColorShader.h"

CSetColorShader::CSetColorShader()
    : CComputeShader(32, 32, 1)
{
}

CSetColorShader::~CSetColorShader()
{
}

int CSetColorShader::UpdateData()
{
    if (nullptr == m_TargetTex)
        return E_FAIL;

    // 상수 세팅
    m_Const.arrInt[0] = m_TargetTex->GetWidth();
    m_Const.arrInt[1] = m_TargetTex->GetHeight();
    m_Const.arrVec4[0] = m_Color;

    // 텍스쳐 U0 에 바인딩
    if (FAILED(m_TargetTex->UpdateData_CS_UAV(0)))
    {
        return E_FAIL;
    }

    // 스레드 그룹 수 체크
    UINT width = m_TargetTex->GetWidth();
    UINT height = m_TargetTex->GetHeight();

    SetGroupX(1 + width / m_ThreadX);
    SetGroupY(1 + height / m_ThreadY);
    SetGroupZ(1);

    return S_OK;
}

void CSetColorShader::Clear()
{
    m_TargetTex->Clear_CS_UAV();
}
