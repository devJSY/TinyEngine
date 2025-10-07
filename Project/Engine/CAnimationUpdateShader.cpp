#include "pch.h"
#include "CAnimationUpdateShader.h"

CAnimationUpdateShader::CAnimationUpdateShader()
    : CComputeShader(256, 1, 1)
    , m_pFrameDataBuffer(nullptr)
    , m_pOffsetMatBuffer(nullptr)
    , m_pBoneTransformMatBuffer(nullptr)
    , m_pFinalMatBuffer(nullptr)
    , m_pPrevFinalMatBuffer(nullptr)
{
    Create(L"shader/animation_update.fx", "AnimationUpdateCS");
}

CAnimationUpdateShader::~CAnimationUpdateShader()
{
}

int CAnimationUpdateShader::UpdateData()
{
    // ����ȭ���� ����
    m_pFrameDataBuffer->UpdateData_CS_SRV(32);       // t32
    m_pOffsetMatBuffer->UpdateData_CS_SRV(33);       // t33
    m_pBoneTransformMatBuffer->UpdateData_CS_UAV(0); // u0
    m_pFinalMatBuffer->UpdateData_CS_UAV(1);         // u1
    m_pPrevFinalMatBuffer->UpdateData_CS_UAV(2);     // u2

    return S_OK;
}

void CAnimationUpdateShader::UpdateGroupCount()
{
    UINT GroupX = (m_Const.arrInt[0] / m_ThreadX) + 1;
    UINT GroupY = 1;
    UINT GroupZ = 1;

    SetGroup(GroupX, GroupY, GroupZ);
}

void CAnimationUpdateShader::Clear()
{
    m_pFrameDataBuffer->Clear_CS_SRV();
    m_pOffsetMatBuffer->Clear_CS_SRV();
    m_pBoneTransformMatBuffer->Clear_CS_UAV();
    m_pFinalMatBuffer->Clear_CS_UAV();
    m_pPrevFinalMatBuffer->Clear_CS_UAV();

    m_pFrameDataBuffer = nullptr;
    m_pOffsetMatBuffer = nullptr;
    m_pBoneTransformMatBuffer = nullptr;
    m_pFinalMatBuffer = nullptr;
    m_pPrevFinalMatBuffer = nullptr;
}