#include "pch.h"
#include "CExitButton.h"

CExitButton::CExitButton()
    : CButtonScript(EXITBUTTON)
{
}

CExitButton::~CExitButton()
{
}

void CExitButton::Func()
{
    PostQuitMessage(0);
}