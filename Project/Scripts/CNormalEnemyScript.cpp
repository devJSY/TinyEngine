#include "pch.h"
#include "CNormalEnemyScript.h"

CNormalEnemyScript::CNormalEnemyScript()
    : CMonsterUnitScript(NORMALENEMYSCRIPT)
    , m_eState(NORMALENEMY_STATE::Idle)
{
}

CNormalEnemyScript::~CNormalEnemyScript()
{
}

void CNormalEnemyScript::begin()
{
}

void CNormalEnemyScript::tick()
{
    switch (m_eState)
    {
    case NORMALENEMY_STATE::Idle:
    {
        Idle();
    }
        break;
    case NORMALENEMY_STATE::Sleep:
    {

    }
        break;
    case NORMALENEMY_STATE::Patrol:
        break;
    case NORMALENEMY_STATE::Find:
        break;
    case NORMALENEMY_STATE::Attack:
        break;
    case NORMALENEMY_STATE::AttackSuccessed:
        break;
    case NORMALENEMY_STATE::AttackFailed:
        break;
    case NORMALENEMY_STATE::End:
        break;
    default:
        break;
    }
}

void CNormalEnemyScript::ChangeState(NORMALENEMY_STATE _state)
{
    EnterState(_state);
    NORMALENEMY_STATE _prev = m_eState;
    m_eState = _state;
    ExitState(_prev);
}

void CNormalEnemyScript::EnterState(NORMALENEMY_STATE _state)
{
    switch (_state)
    {
    case NORMALENEMY_STATE::Idle:
    {
        Animator()->Play(MONSTERANIM(L"Wait"));
    }
        break;
    case NORMALENEMY_STATE::Patrol:
    {
        Animator()->Play(M)
    }
        break;
    case NORMALENEMY_STATE::Find:
        break;
    case NORMALENEMY_STATE::Attack:
        break;
    case NORMALENEMY_STATE::AttackSuccessed:
        break;
    case NORMALENEMY_STATE::AttackFailed:
        break;
    case NORMALENEMY_STATE::End:
        break;
    default:
        break;
    }
}

void CNormalEnemyScript::ExitState(NORMALENEMY_STATE _state)
{
    switch (_state)
    {
    case NORMALENEMY_STATE::Idle:
        break;
    case NORMALENEMY_STATE::Patrol:
        break;
    case NORMALENEMY_STATE::Find:
        break;
    case NORMALENEMY_STATE::Attack:
        break;
    case NORMALENEMY_STATE::AttackSuccessed:
        break;
    case NORMALENEMY_STATE::AttackFailed:
        break;
    case NORMALENEMY_STATE::End:
        break;
    default:
        break;
    }
}

void CNormalEnemyScript::Idle()
{

}

void CNormalEnemyScript::SaveToLevelFile(FILE* _File)
{
}

void CNormalEnemyScript::LoadFromLevelFile(FILE* _File)
{
}