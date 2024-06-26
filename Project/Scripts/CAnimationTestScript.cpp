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
        Animator()->Play(L"Armature|ArenaDeathWait");
    }
    if (KEY_TAP(KEY::NUM2))
    {
        Animator()->Play(L"Armature|Flight");
    }
    if (KEY_TAP(KEY::NUM3))
    {
        Animator()->Play(L"Armature|NormalExit");
    }
    if (KEY_TAP(KEY::NUM4))
    {
        Animator()->Play(L"Armature|SuperJumpRolling");
    }

    if (KEY_TAP(KEY::F))
    {
        LOG(Log, "%d", Animator()->IsFinish())
    }
}
