#include "pch.h"
#include "CGordoLongScript.h"

CGordoLongScript::CGordoLongScript()
    : CMonsterUnitScript(GORDOLONGSCRIPT)
{
}

CGordoLongScript::~CGordoLongScript()
{
}

void CGordoLongScript::begin()
{
    Animator()->Play(ANIMPREFIX(L" Wait"));
}

void CGordoLongScript::tick()
{
    TransformRotate();
}

void CGordoLongScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CGordoLongScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}