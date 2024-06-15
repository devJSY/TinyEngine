#include "pch.h"
#include "CCopyBoneShader.h"

CCopyBoneShader::CCopyBoneShader()
    : CComputeShader(256, 1, 1)
    , m_SrcBuffer(nullptr)
    , m_DestBuffer(nullptr)
{
    Create(L"shader\\copybone.fx", "CS_CopyBoneMatrix");
}

CCopyBoneShader::~CCopyBoneShader()
{
}

int CCopyBoneShader::UpdateData()
{
    // 구조화버퍼 전달
    m_SrcBuffer->UpdateData_CS_SRV(34); // t34
    m_DestBuffer->UpdateData_CS_UAV(0); // u0

    return S_OK;
}

void CCopyBoneShader::UpdateGroupCount()
{
    // 그룹 수 계산
    int iBoneCount = m_Const.arrInt[0];

    UINT GroupX = iBoneCount / m_ThreadX + 1;
    UINT GroupY = 1;
    UINT GroupZ = 1;

    SetGroup(GroupX, GroupY, GroupZ);
}

void CCopyBoneShader::Clear()
{
    // 전달한 구조화버퍼 클리어
    m_SrcBuffer->Clear_CS_SRV();
    m_DestBuffer->Clear_CS_UAV();
}
