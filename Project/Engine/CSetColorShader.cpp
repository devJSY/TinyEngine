#include "pch.h"
#include "CSetColorShader.h"

CSetColorShader::CSetColorShader()
    : CComputeShader(32, 32, 1)
{
    Create(L"shader/setcolor.fx", "CS_SetColor");
}

CSetColorShader::~CSetColorShader()
{
}

int CSetColorShader::UpdateData()
{
    if (nullptr == m_TargetTex)
        return E_FAIL;

    // ��� ����
    m_Const.arrInt[0] = m_TargetTex->GetWidth();
    m_Const.arrInt[1] = m_TargetTex->GetHeight();
    m_Const.arrVec4[0] = m_Color;

    // �ؽ��� U0 �� ���ε�
    if (FAILED(m_TargetTex->UpdateData_CS_UAV(0)))
    {
        return E_FAIL;
    }

    return S_OK;
}

void CSetColorShader::UpdateGroupCount()
{
    // ������ �׷� �� üũ
    UINT width = m_TargetTex->GetWidth();
    UINT height = m_TargetTex->GetHeight();

    SetGroupX(1 + width / m_ThreadX);
    SetGroupY(1 + height / m_ThreadY);
    SetGroupZ(1);
}

void CSetColorShader::Clear()
{
    m_TargetTex->Clear_CS_UAV();
}
