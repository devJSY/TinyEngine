#include "pch.h"
#include "CCinematicScript.h"

#include "CPlayerScript.h"
#include "CPlayerCameraScript.h"

CCinematicScript::CCinematicScript()
    : CScript(CINEMATICSCRIPT)
{
}

CCinematicScript::~CCinematicScript()
{
}

void CCinematicScript::tick()
{
    if (KEY_TAP(KEY::O))
    {
        CGameObject* pPlayerCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerCamera");
        if (nullptr != pPlayerCamObj)
        {
            CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
            if (nullptr != pScript)
            {
                pScript->Lock(false);
            }
        }
    }
}

void CCinematicScript::OnTriggerEnter(CCollider2D* _OtherCollider)
{
    if (L"Player" != _OtherCollider->GetOwner()->GetName())
        return;

    CGameObject* pPlayerCamObj = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"PlayerCamera");
    if (nullptr != pPlayerCamObj)
    {
        CPlayerCameraScript* pScript = pPlayerCamObj->GetScript<CPlayerCameraScript>();
        if (nullptr != pScript)
        {
            pScript->Lock(true, Transform()->GetWorldPos());
        }
    }

    CPlayerScript* pPlayerScript = CGameManagerScript::GetInset()->GetPlayer()->GetScript<CPlayerScript>();
    if (nullptr != pPlayerScript)
    {
        pPlayerScript->ChangeState(PLAYER_STATE::Cinematic);
    }
}
