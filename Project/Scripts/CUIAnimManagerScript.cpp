#include "pch.h"
#include "CUIAnimManagerScript.h"

CUIAnimManagerScript::CUIAnimManagerScript()
    : CScript(UIANIMMANAGERSCRIPT)
    , m_vUIAnimObject{}
{
}

CUIAnimManagerScript::~CUIAnimManagerScript()
{
}

void CUIAnimManagerScript::begin()
{
}

void CUIAnimManagerScript::tick()
{
}

UINT CUIAnimManagerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CUIAnimManagerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
