#include "pch.h"
#include "CKirbyScript.h"

CKirbyScript::CKirbyScript()
    : CUnitScript(KIRBYSCRIPT)
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

CKirbyScript::~CKirbyScript()
{
}
