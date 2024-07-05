#include "pch.h"
#include "CAnimationTestScript.h"

CAnimationTestScript::CAnimationTestScript()
    : CScript(ANIMATIONTESTSCRIPT)
{
}

CAnimationTestScript::~CAnimationTestScript()
{
}

void CAnimationTestScript::tick()
{
    if (nullptr == Animator())
        return;

    if (KEY_TAP(KEY::NUM1))
    {
        Animator()->Play(L"Armature|Move", true);
    }
    if (KEY_TAP(KEY::NUM2))
    {
        Animator()->Play(L"Armature|Wait", true);
    }

    if (KEY_TAP(KEY::F))
    {
        LOG(Log, "%d", Animator()->IsFinish())
    }
}
