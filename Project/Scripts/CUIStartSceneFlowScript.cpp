#include "pch.h"
#include "CUIStartSceneFlowScript.h"
#include "CUIAnimScript.h"

#include "CUIAnimScript.h"

CUIStartSceneFlowScript::CUIStartSceneFlowScript()
    : CUIFlowScript(UISTARTSCENEFLOWSCRIPT)
{
}

CUIStartSceneFlowScript::~CUIStartSceneFlowScript()
{
}

void CUIStartSceneFlowScript::begin()
{
    // TODO : 돌릴 UI만 정하기
    vector<CGameObject*> vObjs = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(15)->GetLayerObjects();

    for (size_t i = 0; i < vObjs.size();i++)
    {
        CUIAnimScript* script = vObjs[i]->GetScript<CUIAnimScript>();
        if (nullptr != script)
        {
            AddFlowScript(script);
        }
    }
}