#include "pch.h"
#include "CKirbyAbility_Sword.h"

CKirbyAbility_Sword::CKirbyAbility_Sword()
    : m_bFrmEnter(true)
{
}

CKirbyAbility_Sword::~CKirbyAbility_Sword()
{
}

// ===============
// Attack
// ===============
// 칼휘두르기 (Lv1)

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
    PLAYER->Animator()->Play(KIRBYANIM(L"SideSlash"), false);

    m_bFrmEnter = true;

    // Effect 재생
}

void CKirbyAbility_Sword::AttackExit()
{
}

// End
void CKirbyAbility_Sword::AttackEnd()
{
}

void CKirbyAbility_Sword::AttackEndEnter()
{
    PLAYER->Animator()->Play(KIRBYANIM(L"SideSlashEnd"), false);
}

void CKirbyAbility_Sword::AttackEndExit()
{
}
