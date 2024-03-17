#include "pch.h"
#include "CPlayButtonScript.h"
#include <Engine\\CLevelMgr.h>
#include <Engine\\CLevel.h>

#include "CLevelChangeScript.h"

CPlayButtonScript::CPlayButtonScript()
    : CButtonScript(PLAYBUTTONSCRIPT)
{
}

CPlayButtonScript::~CPlayButtonScript()
{
}

void CPlayButtonScript::tick()
{
    CButtonScript::tick();

    if (m_bClicked)
    {
        CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
        CGameObject* pGMObj = pCurLevel->FindObjectByName(L"GameManager");

        if (nullptr == pGMObj)
            return;

        CLevelChangeScript* pScript = pGMObj->GetScript<CLevelChangeScript>();
        if (nullptr != pScript)
            pScript->ChangeLevel("Hall of Eternity Floor 0");
    }
}
