#include "pch.h"
#include "CKirbyUnitScript.h"

CKirbyUnitScript::CKirbyUnitScript()
    : CUnitScript(KIRBYUNITSCRIPT)
{
    UnitInfo KirbyInfo =
    {
        100.f, // HP
        10.f, // Speed
        10.f, //JumpPower
        100.f, //MaxHP
    };

    SetInfo(KirbyInfo);
}

CKirbyUnitScript::~CKirbyUnitScript()
{
}