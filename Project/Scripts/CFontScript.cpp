#include "pch.h"
#include "CFontScript.h"
#include <Engine\\CFontMgr.h>

CFontScript::CFontScript()
    : CScript(FONTSCRIPT)
    , m_FontString()
    , m_FontPos()
    , m_FontSize()
    , m_FontColor()
{
    AddScriptParam(SCRIPT_PARAM::STRING, &m_FontString, "Font String");
    AddScriptParam(SCRIPT_PARAM::VEC2, &m_FontPos, "Font Position");
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_FontSize, "Font Size");
    AddScriptParam(SCRIPT_PARAM::VEC4, &m_FontColor, "Font Color");
}

CFontScript::~CFontScript()
{
}

void CFontScript::tick()
{
    UINT color = FONT_RGBA(m_FontColor.x, m_FontColor.y, m_FontColor.z, m_FontColor.w);
    CFontMgr::GetInst()->DrawFont(tFont{ToWstring(m_FontString), m_FontPos.x, m_FontPos.y, m_FontSize, color});
}

void CFontScript::SaveToLevelFile(FILE* _File)
{
    SaveStringToFile(m_FontString, _File);
    fwrite(&m_FontPos, sizeof(Vec2), 1, _File);
    fwrite(&m_FontSize, sizeof(float), 1, _File);
    fwrite(&m_FontColor, sizeof(Vec4), 1, _File);
}

void CFontScript::LoadFromLevelFile(FILE* _File)
{
    LoadStringFromFile(m_FontString, _File);
    fread(&m_FontPos, sizeof(Vec2), 1, _File);
    fread(&m_FontSize, sizeof(float), 1, _File);
    fread(&m_FontColor, sizeof(Vec4), 1, _File);
}
