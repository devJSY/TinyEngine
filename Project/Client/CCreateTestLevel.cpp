#include "pch.h"
#include "CCreateTestLevel.h"

#include <Engine/func.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CScript.h>

#include <Engine/CLevelMgr.h>
#include <Scripts/CScriptMgr.h>

CLevel* CCreateTestLevel::CreateTestLevel()
{
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    pCurLevel->SetName(L"Test Level");

    for (int i = 0; i < LAYER_MAX; i++)
    {
        wstring Name = L"Layer ";
        Name += std::to_wstring(i);
        pCurLevel->GetLayer(i)->SetName(Name);
    }

    CGameObject* pCamObj = new CGameObject;
    pCamObj->SetName(L"Main Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(0);
    pCamObj->Camera()->LayerMaskAll();
    pCamObj->Camera()->LayerMask(pCurLevel, L"UI", false);
    pCamObj->Camera()->SetHDRI(false);

    pCurLevel->AddObject(pCamObj, 0);

    // UI ¸¸ ·»´õ¸µ
    pCamObj = new CGameObject;
    pCamObj->SetName(L"UI Camera");
    pCamObj->AddComponent(new CTransform);
    pCamObj->AddComponent(new CCamera);

    pCamObj->Camera()->SetCameraPriority(1);
    pCamObj->Camera()->LayerMask(pCurLevel, L"UI", true);

    pCurLevel->AddObject(pCamObj, 0);

    return pCurLevel;
}
