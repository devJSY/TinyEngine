#include "pch.h"
#include "CKirbyIdle.h"

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
        KirbyFSM->GetCurAbility()->Idle();
    }

    // 기본적으로 수행해야 하는 동작은 여기가 아닌듯;
    // Change State
    // ex, Change State를 여기서 수행하면 ...
    // (1) 구현 범위 헷갈림
    // (2) 위쪽에서 Change State 발생하고 또다시 발생하는 경우 의도한대로 상태가 안 도는 경우 생김
    // - ex 특정상태Start && 상태전환 -> 특정상태~End까지 의도한대로 동작 않을 수? 있 ? 음 ?
}

void CKirbyIdle::Enter()
{
    PLAY_CURSTATE(IdleEnter)
}

void CKirbyIdle::Exit()
{
    PLAY_CURSTATE(IdleExit)
}