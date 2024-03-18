#include "pch.h"
#include "CTextRender.h"
#include "CFontMgr.h"

CTextRender::CTextRender()
    : CRenderComponent(COMPONENT_TYPE::TEXTRENDER)
    , m_Text()
    , m_Position()
    , m_Size()
    , m_Color()
{
    SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
    SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl")); // Not Use
}

CTextRender::~CTextRender()
{
}

void CTextRender::render()
{
    CFontMgr::GetInst()->DrawFont(m_Text.c_str(), m_Position.x, m_Position.y, m_Size,
                                  FONT_RGBA(m_Color.x, m_Color.y, m_Color.z, m_Color.w));
}

void CTextRender::render(Ptr<CMaterial> _mtrl)
{
    render();
}

void CTextRender::SaveToLevelFile(FILE* _File)
{
    SaveWStringToFile(m_Text, _File);
    fwrite(&m_Position, sizeof(Vec2), 1, _File);
    fwrite(&m_Size, sizeof(float), 1, _File);
    fwrite(&m_Color, sizeof(Vec4), 1, _File);
}

void CTextRender::LoadFromLevelFile(FILE* _File)
{
    LoadWStringFromFile(m_Text, _File);
    fread(&m_Position, sizeof(Vec2), 1, _File);
    fread(&m_Size, sizeof(float), 1, _File);
    fread(&m_Color, sizeof(Vec4), 1, _File);
}
