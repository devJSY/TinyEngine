#include "pch.h"
#include "CLightBulbGimmickScript.h"

CLightBulbGimmickScript::CLightBulbGimmickScript()
    : CScript(LIGHTBULBGIMMICKSCRIPT)
{
}

CLightBulbGimmickScript::~CLightBulbGimmickScript()
{
}

void CLightBulbGimmickScript::begin()
{
    Animator()->Play(ANIMPREFIX("Wait"), false);
}

UINT CLightBulbGimmickScript::SaveToLevelFile(FILE* _File)
{
    return 0;
}

UINT CLightBulbGimmickScript::LoadFromLevelFile(FILE* _File)
{
    return 0;
}
