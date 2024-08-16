#include "pch.h"
#include "CUIAnimManagerScript.h"

#include "CUIAnimScript.h"

CUIAnimManagerScript::CUIAnimManagerScript()
    : CScript(UIANIMMANAGERSCRIPT)
    , m_vAnimScript{}
{
}

CUIAnimManagerScript::~CUIAnimManagerScript()
{
}

void CUIAnimManagerScript::begin()
{
    vector<CGameObject*> vObj = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(LAYER_UI)->GetLayerObjects();

    for (size_t i = 0; i < vObj.size(); i++)
    {
        CUIAnimScript* pScript = vObj[i]->GetScript<CUIAnimScript>();
        if (nullptr != pScript)
        {
            m_vAnimScript.push_back(pScript);
            pScript->SetUIAnimState(UIAnimState::PrePared);
        }
    }
}

void CUIAnimManagerScript::tick()
{
    for (size_t i = 0; i < m_vAnimScript.size(); i++)
    {
        if (nullptr != m_vAnimScript[i])
        {
            m_vAnimScript[i]->tick();
        }
    }
}

void CUIAnimManagerScript::EndAnimUI()
{
    for (size_t i = 0; i < m_vAnimScript.size(); i++)
    {
        if (nullptr != m_vAnimScript[i])
        {
            m_vAnimScript[i]->SetUIAnimState(UIAnimState::End);
        }
    }
}

bool CUIAnimManagerScript::AllFinishEndUI()
{
    bool bFlag = true;
    for (size_t i = 0; i < m_vAnimScript.size(); i++)
    {
        if (nullptr != m_vAnimScript[i])
        {
            if (!m_vAnimScript[i]->GetFinish())
                bFlag = false;
        }
    }

    return bFlag;
}

UINT CUIAnimManagerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CUIAnimManagerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}