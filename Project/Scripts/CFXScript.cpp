#include "pch.h"
#include "CFXScript.h"

CFXScript::CFXScript()
    : CScript(FXSCRIPT)
{
}

CFXScript::~CFXScript()
{
}

void CFXScript::tick()
{
    if (nullptr != Animator2D())
    {
        if (Animator2D()->IsFinish())
        {
            GamePlayStatic::DestroyGameObject(GetOwner());
        }
    }
}
