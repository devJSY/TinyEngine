#include "pch.h"
#include "CKirbyDropAbility.h"

CKirbyDropAbility::CKirbyDropAbility()
    : m_FrmEnter(true)
{
    //m_Bubble = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\AbilityBubble.pref", L"prefab\\AbilityBubble.pref");
}

CKirbyDropAbility::~CKirbyDropAbility()
{
}

void CKirbyDropAbility::tick()
{
    if (m_FrmEnter && GetOwner()->Animator()->GetClipFrameIndex() == 11)
    {
        // clear current hat & weapon
        GamePlayStatic::DestroyGameObject(PLAYERFSM->GetCurHat());
        GamePlayStatic::DestroyGameObject(PLAYERFSM->GetCurWeapon());
        PLAYERFSM->SetCurHat(nullptr);
        PLAYERFSM->SetCurWeapon(nullptr);

        //@TODO 버블 오브젝트
        // spawn ability buble
        if (nullptr != m_Bubble)
        {
            CGameObject* pBubble = m_Bubble->Instantiate();
            Vec3 InitPos = PLAYER->Transform()->GetWorldPos();

            pBubble->Transform()->SetLocalPos(InitPos);
            GamePlayStatic::SpawnGameObject(pBubble, LAYER_DYNAMIC);
        }

        m_FrmEnter = false;
    }

    // Change State
    if (GetOwner()->Animator()->IsFinish())
    {
        if (GetOwner()->CharacterController()->IsGrounded())
        {
            ChangeState(L"IDLE");
        }
        else
        {
            ChangeState(L"JUMP_FALL");
        }
    }
}

void CKirbyDropAbility::Enter()
{
    PLAYER->Animator()->Play(ANIMPREFIX("AbilityDump"), false);

    m_FrmEnter = true;
}

void CKirbyDropAbility::Exit()
{
}