#include "pch.h"
#include "CUITexScript.h"

#include "CUIManagerScript.h"

CUITexScript::CUITexScript()
    : CScript(UITEXSCRIPT)
    , m_vTexName{}
    , m_iPrevTex(0)
    , m_iCurTex(0)
    , m_iCurTexCount(5)
{
    m_vTexName.resize(m_iCurTexCount);
    for (UINT i = 0; i < m_iCurTexCount; i++)
    {
        AddScriptParam(SCRIPT_PARAM::STRING, &m_vTexName[i], "TextureCount" + to_string(i));
    }
}

CUITexScript::~CUITexScript()
{
}

void CUITexScript::begin()
{
    if (m_pMtrl != nullptr)
        return;

    m_pMtrl = GetOwner()->GetComponent<CMeshRender>()->GetMaterial(0);
}

void CUITexScript::tick()
{
    if (m_iPrevTex != m_iCurTex)
    {
        m_pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->Load<CTexture>(L"Texture\\UILogo\\RobbyLevel\\" + ToWstring(m_vTexName[m_iCurTex]),
                                                                         L"Texture\\UILogo\\RobbyLevel\\" + ToWstring(m_vTexName[m_iCurTex])));
        m_iPrevTex = m_iCurTex;
    }
}

UINT CUITexScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fwrite(&m_iCurTexCount, sizeof(UINT), 1, _File);

    for (UINT i = 0; i < m_iCurTexCount; i++)
    {
        MemoryByte += SaveWStringToFile(ToWstring(m_vTexName[i]), _File);
    }

    MemoryByte += sizeof(UINT);

    return MemoryByte;
}

UINT CUITexScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    fread(&m_iCurTexCount, sizeof(UINT), 1, _File);

    for (UINT i = 0; i < m_iCurTexCount; i++)
    {
        wstring _TexName = L"";
        MemoryByte += LoadWStringFromFile(_TexName, _File);
        m_vTexName[i] = ToString(_TexName);
    }

    MemoryByte += sizeof(UINT);

    return MemoryByte;
}