#include "pch.h"
#include "CKirbyIdle.h"

// 통?합?
#include "CKirbyFSM.h"
#include "CPlayerMgr.h"
#include "CKirbyObject.h"
#include "CKirbyAbility.h"

CKirbyIdle::CKirbyIdle()
{
}

CKirbyIdle::~CKirbyIdle()
{
}

void CKirbyIdle::tick()
{
    CKirbyFSM* KirbyFSM = CPlayerMgr::GetPlayerFSM();
    // CKirbyFSM* KirbyFSM = dynamic_cast<CKirbyFSM*>(GetOwnerFSM());
    // PlayerManager 따위를 둠 -> manager가 player를 들고 있고, 그걸 가져오는게 나을듯 (level 또는 게임 시작시 1회 찾음)

    // CASE: Object Copy인 경우 (우선)
    if (KirbyFSM->GetCurObject())
    {
        KirbyFSM->GetCurObject()->Idle();
    }

    // CASE: Ability Copy인 경우
    else
    {
        // 현재 state에 맞는 추가동작
        //KirbyFSM->GetCurAbility()->Idle();
    }

    // 기본적으로 수행해야 하는 동작

    // Change State
    if (KEY_TAP(KEY::Q))
        ChangeState(L"ATTACK");
    else if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
        ChangeState(L"RUN");
}

void CKirbyIdle::Enter()
{
}

void CKirbyIdle::Exit()
{
}