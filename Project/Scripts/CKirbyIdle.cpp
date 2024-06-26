#include "pch.h"
#include "CKirbyIdle.h"
#include "CKirbyFSM.h"
#include "CPlayerMgr.h"

#define KEY_TAP_ARROW (KEY_TAP(KEY::UP) || KEY_TAP(KEY::DOWN) || KEY_TAP(KEY::LEFT) || KEY_TAP(KEY::RIGHT))
#define KEY_PRESSED_ARROW (KEY_PRESSED(KEY::UP) || KEY_PRESSED(KEY::DOWN) || KEY_PRESSED(KEY::LEFT) || KEY_PRESSED(KEY::RIGHT))

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

    // 기본적으로 수행해야 하는 동작
    if (KEY_TAP_ARROW || KEY_PRESSED_ARROW)
        ChangeState(L"RUN");
}

void CKirbyIdle::Enter()
{
}

void CKirbyIdle::Exit()
{
}