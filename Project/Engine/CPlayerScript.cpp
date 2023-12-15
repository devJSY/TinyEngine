#include "pch.h"
#include "CPlayerScript.h"

CPlayerScript::CPlayerScript()
    : m_Speed(2.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::tick()
{
    Vec3 vPos = Transform()->GetRelativePos();

    if (KEY_PRESSED(KEY::W))
    {
        vPos.y += DT * m_Speed;
    }

    if (KEY_PRESSED(KEY::S))
    {
        vPos.y -= DT * m_Speed;
    }

    if (KEY_PRESSED(KEY::A))
    {
        vPos.x -= DT * m_Speed;
    }

    if (KEY_PRESSED(KEY::D))
    {
        vPos.x += DT * m_Speed;
    }

    Transform()->SetRelativePos(vPos);
}
