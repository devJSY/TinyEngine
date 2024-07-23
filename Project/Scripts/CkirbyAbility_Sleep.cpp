#include "pch.h"
#include "CkirbyAbility_Sleep.h"

CkirbyAbility_Sleep::CkirbyAbility_Sleep()
{
    m_Hat = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbySleepHat.pref", L"prefab\\KirbySleepHat.pref");
}

CkirbyAbility_Sleep::~CkirbyAbility_Sleep()
{
}

void CkirbyAbility_Sleep::IdleEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepWait"));
}

void CkirbyAbility_Sleep::IdleStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepWaitStart"), false);
}

void CkirbyAbility_Sleep::RunEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepWalk"));
}

void CkirbyAbility_Sleep::JumpFallEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepFall"));
}

void CkirbyAbility_Sleep::AttackEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("Sleep"), true);
}

void CkirbyAbility_Sleep::AttackStartEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepStart"), false);

    //@TODO SnotBubble








}

void CkirbyAbility_Sleep::AttackEndEnter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("SleepEnd"), false);
}