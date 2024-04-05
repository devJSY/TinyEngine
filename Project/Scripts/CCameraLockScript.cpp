#include "pch.h"
#include "CCameraLockScript.h"
#include "CPlayerCameraScript.h"

CCameraLockScript::CCameraLockScript()
    : CScript(CAMERALOCKSCRIPT)
{
}

CCameraLockScript::~CCameraLockScript()
{
}

void CCameraLockScript::tick()
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

void CCameraLockScript::OnTriggerEnter(CCollider2D* _OtherCollider)
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
}
