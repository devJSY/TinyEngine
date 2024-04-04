#include "pch.h"
#include "CBridgeScript.h"

CBridgeScript::CBridgeScript()
    : CScript(BRIDGESCRIPT)
    , m_bEnabled(false)
    , m_PassedTime(0.f)
{
}

CBridgeScript::~CBridgeScript()
{
}
void CBridgeScript::begin()
{
    SetEnabled(false);
}

void CBridgeScript::tick()
{
    if (m_bEnabled)
    {
        m_PassedTime += DT;
    }

    if (m_PassedTime > 0.3f)
    {
        SetEnabled(false);
    }
}

void CBridgeScript::SetEnabled(bool _bEnable)
{
    m_bEnabled = _bEnable;
    m_PassedTime = 0.f;

    if (nullptr == BoxCollider2D())
        return;

    BoxCollider2D()->SetEnabled(_bEnable);
}
