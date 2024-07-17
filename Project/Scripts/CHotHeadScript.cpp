#include "pch.h"
#include "CHotHeadScript.h"

CHotHeadScript::CHotHeadScript()
    : CMonsterUnitScript(HOTHEADSCRIPT)
    , m_eState(HOTHEAD_STATE::Idle)
{
}

CHotHeadScript::~CHotHeadScript()
{
}

void CHotHeadScript::begin()
{
}

void CHotHeadScript::tick()
{
    ChangeState(HOTHEAD_STATE::Damage);
}

void CHotHeadScript::EnterState(HOTHEAD_STATE _state)
{
    switch (_state)
    {
    case HOTHEAD_STATE::Idle:
        break;
    case HOTHEAD_STATE::Damage: {
        Vec3 vDir = Transform()->GetWorldDir(DIR_TYPE::FRONT);
        vDir.y = 0.f;
        Rigidbody()->AddForce(-vDir * 300.f, ForceMode::Impulse);
    }
        break;
    case HOTHEAD_STATE::End:
        break;
    default:
        break;
    }
}

void CHotHeadScript::ChangeState(HOTHEAD_STATE _state)
{
    ExitState(m_eState);
    m_eState = _state;
    EnterState(m_eState);
}

void CHotHeadScript::ExitState(HOTHEAD_STATE _state)
{
    switch (_state)
    {
    case HOTHEAD_STATE::Idle:
        break;
    case HOTHEAD_STATE::Damage:
        break;
    case HOTHEAD_STATE::End:
        break;
    default:
        break;
    }
}

void CHotHeadScript::SaveToLevelFile(FILE* _File)
{
    CMonsterUnitScript::SaveToLevelFile(_File);
}

void CHotHeadScript::LoadFromLevelFile(FILE* _File)
{
    CMonsterUnitScript::LoadFromLevelFile(_File);
}