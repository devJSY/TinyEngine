#include "pch.h"
#include "CBridgeScript.h"

CBridgeScript::CBridgeScript()
    : CScript(BRIDGESCRIPT)
    , m_DisabledTime(0.f)
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
    if (m_DisabledTime > 0.f)
        m_DisabledTime -= DT;
}

void CBridgeScript::SetEnabled(bool _bEnable, bool ForceEnable)
{
    if (_bEnable)
    {
        // Enable 은 DisabledTime이 지난 경우에만 적용 - 강제 활성화 인경우는 무시
        if (!ForceEnable && m_DisabledTime > 0.f)
            return; 
    }
    else
        m_DisabledTime = 0.3f;

    if (nullptr == BoxCollider2D())
        return;

    BoxCollider2D()->SetEnabled(_bEnable);
}
