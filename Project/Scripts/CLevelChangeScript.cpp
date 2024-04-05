#include "pch.h"
#include "CLevelChangeScript.h"
#include <Engine\\CRenderMgr.h>
#include <Engine\\CAnim.h>
#include <Engine\\CLevelSaveLoad.h>
#include <Engine\\components.h>
#include <Engine\\CDevice.h>
#include <Engine\\CEditorMgr.h>

CLevelChangeScript::CLevelChangeScript()
    : CScript(LEVELCHANGESCRIPT)
    , m_ChangeLevelName()
    , m_TransitionFilterObj(nullptr)
{
}

CLevelChangeScript::~CLevelChangeScript()
{
}

void CLevelChangeScript::tick()
{
    if (m_ChangeLevelName.empty())
        return;

    if (nullptr != m_TransitionFilterObj && m_TransitionFilterObj->Animator2D()->IsFinish())
    {
        GamePlayStatic::ChangeLevel(CLevelSaveLoad::LoadLevel(ToWstring(m_ChangeLevelName)), LEVEL_STATE::PLAY);
    }
}

void CLevelChangeScript::ChangeLevel(const std::string& LevelName)
{
    if (!m_ChangeLevelName.empty()) // 레벨 변경중인 상태
        return;

    m_ChangeLevelName = LevelName;

    int PostProcessIdx = LAYER_MAX - 1;
    CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

    for (int i = 0; i < LAYER_MAX; i++)
    {
        if (L"PostProcess" == pCurLevel->GetLayer(i)->GetName())
        {
            PostProcessIdx = i;
            break;
        }
    }

    m_TransitionFilterObj = CAssetMgr::GetInst()->Load<CPrefab>(L"prefab\\TransitionFilter.pref", L"prefab\\TransitionFilter.pref")->Instantiate();
    GamePlayStatic::SpawnGameObject(m_TransitionFilterObj, PostProcessIdx);
}
