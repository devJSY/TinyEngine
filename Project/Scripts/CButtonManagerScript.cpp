#include "pch.h"
#include "CButtonManagerScript.h"

#include <Engine/CLevelMgr.h>

#include <Engine/CGameObject.h>
#include "CButtonScript.h"

CButtonManagerScript::CButtonManagerScript()
    : CScript(UIMANAGERSCRIPT)
    , m_vBtn{}
    , m_iCurBtn(0)
    , m_iPrevBtn(0)
{
    AddScriptParam(SCRIPT_PARAM::INT, &m_iCurBtn, "CurrentButton");
}

CButtonManagerScript::~CButtonManagerScript()
{
}

void CButtonManagerScript::begin()
{
    if (m_vBtn.size() != 0)
        return;

    CLevel* _pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    vector<CGameObject*> _vBtnObject = _pCurLevel->GetLayer(15)->GetLayerObjects();

    vector<CGameObject*>::iterator iter = _vBtnObject.begin();

    for (; iter != _vBtnObject.end(); iter++)
    {
        CButtonScript* _script = (*iter)->GetScript<CButtonScript>();

        if (nullptr != _script)
        {
            // Test
            _script->SetTransition(ButtonTransition::SCALE);
            _script->SetScale(ButtonState::NORMAL, Vec2(1.f, 1.f));
            _script->SetScale(ButtonState::SELECTED, Vec2(1.5f, 1.5f));
            m_vBtn.push_back(_script);
        }
    }
}

void CButtonManagerScript::tick()
{
    if (!m_vBtn.empty())
    {
        ButtonManipulation();
    }

    if (m_iPrevBtn != m_iCurBtn)
    {
        m_vBtn[m_iPrevBtn]->ChangeState(ButtonState::NORMAL);
        m_vBtn[m_iCurBtn]->ChangeState(ButtonState::SELECTED);

        m_iPrevBtn = m_iCurBtn;
    }

    if (KEY_TAP(SPACE) || (KEY_TAP(LBTN) && m_vBtn[m_iCurBtn]->IsHovered()))
    {
        // m_vBtn[m_iCurBtn]->Func();
    }
}

void CButtonManagerScript::ButtonManipulation()
{
    if (KEY_TAP(UP))
    {
        if (m_iCurBtn != 0)
        {
            m_iCurBtn--;
        }
    }

    if (KEY_TAP(DOWN))
    {
        if (m_iCurBtn != m_vBtn.size() - 1)
        {
            m_iCurBtn++;
        }
    }

    for (size_t i = 0; i < m_vBtn.size(); i++)
    {
        if (m_vBtn[i]->IsHovered() && m_vBtn[i]->IsInteraction())
        {
            m_iCurBtn = static_cast<UINT>(i);
        }
    }
}