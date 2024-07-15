#include "pch.h"
#include "CPlayerTestScript.h"

CPlayerTestScript::CPlayerTestScript()
    : CScript(PLAYERTESTSCRIPT)
    , m_fSpeed(5.f)
{
    AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "Speed");
}

CPlayerTestScript::~CPlayerTestScript()
{
}

void CPlayerTestScript::tick()
{
    Vec3 _vPos = Transform()->GetLocalPos();
    Vec3 _vDir = {};
    Quaternion _quat = {};

    if (KEY_PRESSED(UP))
    {
        _vDir += Vec3(0.f, 0.f, 1.f);
    }

    if (KEY_PRESSED(DOWN))
    {
        _vDir += Vec3(0.f, 0.f, -1.f);
    }

    if (KEY_PRESSED(RIGHT))
    {
        _vDir += Vec3(1.f, 0.f, 0.f);
    }

    if (KEY_PRESSED(LEFT))
    {
        _vDir += Vec3(-1.f, 0.f, 0.f);
    }

    _vDir.Normalize();

    _quat = Quat::LookRotation(_vDir, Vec3(0.f, 1.f, 0.f));

    Transform()->SetWorldRotation(_quat);
    Transform()->SetLocalPos(_vPos + _vDir * m_fSpeed * DT);
}