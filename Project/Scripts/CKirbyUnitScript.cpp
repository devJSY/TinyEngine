#include "pch.h"
#include "CKirbyUnitScript.h"

CKirbyUnitScript::CKirbyUnitScript()
    : CUnitScript(KIRBYUNITSCRIPT)
{
    UnitInfo KirbyInfo =
    {
        100.f, // HP
        20.f, // Speed
        50.f, //JumpPower
        100.f, //MaxHP
    };

    SetInfo(KirbyInfo);
}

CKirbyUnitScript::~CKirbyUnitScript()
{
}