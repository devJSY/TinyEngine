#include "pch.h"
#include "CAnimation3DShader.h"

CAnimation3DShader::CAnimation3DShader()
    : CComputeShader(256, 1, 1)
    , m_pFrameDataBuffer(nullptr)
    , m_pOffsetMatBuffer(nullptr)
    , m_pOutputBuffer(nullptr)
{
    Create(L"shader\\animation.fx", "CS_Animation3D");
}

CAnimation3DShader::~CAnimation3DShader()
{
}

int CAnimation3DShader::UpdateData()
{
    // 구조화버퍼 전달
    m_pFrameDataBuffer->UpdateData_CS_SRV(32); // t32
    m_pOffsetMatBuffer->UpdateData_CS_SRV(33); // t33
    m_pOutputBuffer->UpdateData_CS_UAV(0);     // u0

    return S_OK;
}

void CAnimation3DShader::UpdateGroupCount()
{
    UINT GroupX = (m_Const.arrInt[0] / m_ThreadX) + 1;
    UINT GroupY = 1;
    UINT GroupZ = 1;

    SetGroup(GroupX, GroupY, GroupZ);
}

void CAnimation3DShader::Clear()
{
    m_pFrameDataBuffer->Clear_CS_SRV();
    m_pOffsetMatBuffer->Clear_CS_SRV();
    m_pOutputBuffer->Clear_CS_UAV();

    m_pFrameDataBuffer = nullptr;
    m_pOffsetMatBuffer = nullptr;
    m_pOutputBuffer = nullptr;
}