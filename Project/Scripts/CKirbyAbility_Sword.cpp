#include "pch.h"
#include "CKirbyAbility_Sword.h"
#include "CKirbyMoveController.h"

CKirbyAbility_Sword::CKirbyAbility_Sword()
    : m_bFrmEnter(true)
{
    m_ComboSuccessTime = 1.f;
}

CKirbyAbility_Sword::~CKirbyAbility_Sword()
{
}

// ===============
// Attack
// ===============
// 칼휘두르기 (Lv0)

void CKirbyAbility_Sword::Attack()
{
    if (GET_CURCLIP_FRM == 7 && m_bFrmEnter)
    {
        CPlayerMgr::ClearBodyMtrl();
        CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));
        m_bFrmEnter = false;

        //@TODO 검기 날리기
        //Ptr<CPrefab> BulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyBullet.pref", L"prefab\\KirbyBullet.pref");
        //if (nullptr != BulletPref)
        //{
        //    CGameObject* BulletInst = BulletPref->Instantiate();
        //}
    }
}

void CKirbyAbility_Sword::AttackEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Attack1"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    m_bFrmEnter = true;

    //@TODO Effect 재생
}

void CKirbyAbility_Sword::AttackExit()
{
    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->SetComboLevel(1);
}

// End
void CKirbyAbility_Sword::AttackEnd()
{
}

void CKirbyAbility_Sword::AttackEndEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"GiantSideSlashEnd"), false);

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyAbility_Sword::AttackEndExit()
{
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();

    PLAYERFSM->SetComboLevel(1);
}

// ===============
// Attack Combo 1
// ===============
// 칼휘두르기 (Lv1)

void CKirbyAbility_Sword::AttackCombo1()
{
    if (GET_CURCLIP_FRM == 9 && m_bFrmEnter)
    {
        CPlayerMgr::ClearBodyMtrl();
        CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));
        m_bFrmEnter = false;

        //@TODO 검기 날리기
        // Ptr<CPrefab> BulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyBullet.pref", L"prefab\\KirbyBullet.pref");
        // if (nullptr != BulletPref)
        //{
        //    CGameObject* BulletInst = BulletPref->Instantiate();
        //}
    }
}

void CKirbyAbility_Sword::AttackCombo1Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Attack2"), false);
    //@TODO Effect 재생

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    m_bFrmEnter = true;
}

void CKirbyAbility_Sword::AttackCombo1Exit()
{
    PLAYERFSM->SetComboLevel(2);

    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

// ===============
// Attack Combo 2
// ===============
// 칼휘두르기 (Lv2)

void CKirbyAbility_Sword::AttackCombo2()
{
    if (GET_CURCLIP_FRM == 17 && m_bFrmEnter)
    {
        CPlayerMgr::ClearBodyMtrl();
        CPlayerMgr::SetPlayerMtrl(PLAYERMESH(BodyVacuum));
        m_bFrmEnter = false;

        //@TODO 검기 날리기
        // Ptr<CPrefab> BulletPref = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\KirbyBullet.pref", L"prefab\\KirbyBullet.pref");
        // if (nullptr != BulletPref)
        //{
        //    CGameObject* BulletInst = BulletPref->Instantiate();
        //}
    }
}

void CKirbyAbility_Sword::AttackCombo2Enter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Attack3"), false);

    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();

    //@TODO Effect 재생
    m_bFrmEnter = true;
}

void CKirbyAbility_Sword::AttackCombo2Exit()
{
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}

// End
void CKirbyAbility_Sword::AttackCombo2End()
{
}

void CKirbyAbility_Sword::AttackCombo2EndEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"Attack3End"), false);

    PLAYERCTRL->LockMove();
    PLAYERCTRL->LockDirection();
    PLAYERCTRL->LockJump();
}

void CKirbyAbility_Sword::AttackCombo2EndExit()
{
    PLAYERFSM->SetComboLevel(0);

    PLAYERCTRL->UnlockMove();
    PLAYERCTRL->UnlockDirection();
    PLAYERCTRL->UnlockJump();
}
