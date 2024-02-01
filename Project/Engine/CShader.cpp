#include "pch.h"
#include "CShader.h"

CShader::CShader(ASSET_TYPE _Type)
    : CAsset(_Type)
    , m_CompileFlags(0)
{
#if defined(DEBUG) || defined(_DEBUG)
    m_CompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
}

CShader::~CShader()
{
}
