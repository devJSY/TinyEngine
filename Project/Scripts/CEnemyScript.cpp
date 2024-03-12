#include "pch.h"
#include "CEnemyScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

CEnemyScript::CEnemyScript()
    : CScript(ENEMYSCRIPT)
    , m_DetectRange(400.f)
    , m_Speed(200.f)
{
}

CEnemyScript::~CEnemyScript()
{
}

void CEnemyScript::begin()
{
    if (StateMachine())
    {
        StateMachine()->AddBlackboardData(L"DetectRange", BLACKBOARD_DATA::FLOAT, &m_DetectRange);
        StateMachine()->AddBlackboardData(L"Speed", BLACKBOARD_DATA::FLOAT, &m_Speed);

        // 플레이어를 찾아서 Object 타입으로 블랙보드에 기록한다.
        CGameObject* pPlayer = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Player");
        if (pPlayer)
        {
            StateMachine()->AddBlackboardData(L"TargetObject", BLACKBOARD_DATA::OBJECT, pPlayer);
        }

        StateMachine()->GetFSM()->SetState(L"IdleState");
    }
}

void CEnemyScript::tick()
{
}

void CEnemyScript::OnCollisionEnter(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnCollisionStay(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnCollisionExit(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnTriggerStay(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::OnTriggerExit(CCollider2D* _OtherCollider)
{
}

void CEnemyScript::SaveToLevelFile(FILE* _File)
{
}

void CEnemyScript::LoadFromLevelFile(FILE* _File)
{
}
