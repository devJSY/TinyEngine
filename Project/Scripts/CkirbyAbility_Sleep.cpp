#include "pch.h"
#include "CkirbyAbility_Sleep.h"
#include "CKirbyMoveController.h"
#include "CKirbyUnitScript.h"
#include "CState.h"

CkirbyAbility_Sleep::CkirbyAbility_Sleep()
    : m_SnotBubble(nullptr)
    , m_PrevSpeed(0.f)
    , m_Speed(2.f)
{
    m_Hat = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySleepHat.pref", L"prefab\\KirbySleepHat.pref");
}

CkirbyAbility_Sleep::~CkirbyAbility_Sleep()
{
}

// ===============
// Idle
// ===============

void CkirbyAbility_Sleep::IdleEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepWait"));
    CPlayerMgr::SetPlayerFace(FaceType::Half);
}

// Start
void CkirbyAbility_Sleep::IdleStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepWaitStart"), false, false, 2.5f, 0.3f);
    CPlayerMgr::SetPlayerFace(FaceType::Half);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CkirbyAbility_Sleep::IdleStartExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

// ===============
// Run
// ===============

void CkirbyAbility_Sleep::RunEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepWalk"), true, false, 2.5f, 0.3);
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthAngryClose));
    CPlayerMgr::SetPlayerFace(FaceType::Close);

    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(m_Speed);
    PLAYERCTRL->LockJump();
}

void CkirbyAbility_Sleep::RunExit()
{
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));
    CPlayerMgr::SetPlayerFace(FaceType::Half);

    PLAYERCTRL->SetSpeed(m_PrevSpeed);
    PLAYERCTRL->UnlockJump();
}

// ===============
// Jump Fall
// ===============

void CkirbyAbility_Sleep::JumpFallEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepFall"));
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthOpen));

    m_PrevSpeed = PLAYERCTRL->GetSpeed();
    PLAYERCTRL->SetSpeed(m_Speed);
}

void CkirbyAbility_Sleep::JumpFallExit()
{
    CPlayerMgr::ClearMouthMtrl();
    CPlayerMgr::SetPlayerMtrl(PLAYERMESH(MouthNormal));

    PLAYERCTRL->SetSpeed(m_PrevSpeed);
}

// ===============
// Attack
// ===============

void CkirbyAbility_Sleep::Attack()
{
    // change animation
    if (m_SnotBubble && m_SnotBubble->Animator()->IsFinish())
    {
        m_SnotBubble->Animator()->Play(ANIMPREFIX("Main"));
    }
}

void CkirbyAbility_Sleep::AttackEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Sleep"), true);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CkirbyAbility_Sleep::AttackExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

// Start
void CkirbyAbility_Sleep::AttackStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepStart"), false);
    CPlayerMgr::SetPlayerFace(FaceType::Close);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    // attach snotBubble
    m_SnotBubble = PLAYER->GetChildObject(L"SleepSnotBubble");
    if (m_SnotBubble)
    {
        m_SnotBubble->SetActive(true);
        m_SnotBubble->Animator()->Play(ANIMPREFIX("BubbleStart"), false);
    }
}

void CkirbyAbility_Sleep::AttackStartExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

// End
void CkirbyAbility_Sleep::AttackEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepEnd"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    if (m_SnotBubble)
    {
        m_SnotBubble->Animator()->Play(ANIMPREFIX("BubbleStart"), false, true);
    }

    PLAYERUNIT->BuffHP(30.f);
}

void CkirbyAbility_Sleep::AttackEndExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    // off snotBubble
    if (m_SnotBubble)
    {
        m_SnotBubble->SetActive(false);
    }
}

// ===============
// Change Ability
// ===============

void CkirbyAbility_Sleep::ChangeAbility()
{
}

void CkirbyAbility_Sleep::ChangeAbilityEnter()
{
    // create hat
    CGameObject* pInstObj = m_Hat->Instantiate();
    PLAYERFSM->SetCurHat(pInstObj);
    GamePlayStatic::AddChildObject(PLAYER, pInstObj, L"Hat");

    pInstObj->Animator()->Play(ANIMPREFIX("Deform"));
}

void CkirbyAbility_Sleep::ChangeAbilityExit()
{
}
