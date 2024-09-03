#include "pch.h"
#include "CUIContinueUIScript.h"

CUIContinueUIScript::CUIContinueUIScript()
    : CScript(UICONTINUEUISCRIPT)
    , m_eState(UIContinueUIState::Progress)
    , m_fWaitTime(1.5f)
    , m_fAccTime(0.f)
{
}

CUIContinueUIScript::CUIContinueUIScript(const CUIContinueUIScript& Origin)
    : CScript(Origin)
    , m_eState(UIContinueUIState::Progress)
    , m_fWaitTime(Origin.m_fWaitTime)
    , m_fAccTime(0.f)
{
}

CUIContinueUIScript::~CUIContinueUIScript()
{
}

void CUIContinueUIScript::begin()
{
    CMeshRender* pRender = MeshRender();
    if (nullptr != pRender)
    {
        pRender->GetMaterial(0)->SetScalarParam(INT_3, 1);
        pRender->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.f);
    }
}

void CUIContinueUIScript::tick()
{
    switch (m_eState)
    {
    case UIContinueUIState::Progress:
        Progress();
        break;
    case UIContinueUIState::End:
        End();
        break;
    default:
        break;
    }
}

void CUIContinueUIScript::Progress()
{
    m_fAccTime += DT;
    if (m_fAccTime >= m_fWaitTime)
    {
        CMeshRender* pRender = MeshRender();
        if (nullptr != pRender)
        {
            pRender->GetMaterial(0)->SetScalarParam(INT_3, 1);
            pRender->GetMaterial(0)->SetScalarParam(FLOAT_1, 1.f);
        }
    }
}

void CUIContinueUIScript::End()
{
    CMeshRender* pRender = MeshRender();
    if (nullptr != pRender)
    {
        pRender->GetMaterial(0)->SetScalarParam(INT_3, 1);
        pRender->GetMaterial(0)->SetScalarParam(FLOAT_1, 0.f);
    }
}

UINT CUIContinueUIScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}

UINT CUIContinueUIScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;
    return MemoryByte;
}