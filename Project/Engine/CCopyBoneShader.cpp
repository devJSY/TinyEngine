#include "pch.h"
#include "CCopyBoneShader.h"

CCopyBoneShader::CCopyBoneShader()
    : CComputeShader(256, 1, 1)
    , m_SrcBuffer(nullptr)
    , m_DestBuffer(nullptr)
{
    Create(L"shader/copybone.fx", "CS_CopyBoneMatrix");
}

CCopyBoneShader::~CCopyBoneShader()
{
}

int CCopyBoneShader::UpdateData()
{
    // ����ȭ���� ����
    m_SrcBuffer->UpdateData_CS_SRV(34); // t34
    m_DestBuffer->UpdateData_CS_UAV(0); // u0

    return S_OK;
}

void CCopyBoneShader::UpdateGroupCount()
{
    // �׷� �� ���
    int iBoneCount = m_Const.arrInt[0];

    UINT GroupX = iBoneCount / m_ThreadX + 1;
    UINT GroupY = 1;
    UINT GroupZ = 1;

    SetGroup(GroupX, GroupY, GroupZ);
}

void CCopyBoneShader::Clear()
{
    // ������ ����ȭ���� Ŭ����
    m_SrcBuffer->Clear_CS_SRV();
    m_DestBuffer->Clear_CS_UAV();
}
