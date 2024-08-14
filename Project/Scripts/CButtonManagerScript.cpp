#include "pch.h"
#include "CButtonManagerScript.h"

#include <Engine/CLevelMgr.h>

#include <Engine/CGameObject.h>
#include "CButtonScript.h"
#include "CStageChangeButtonScript.h"

CButtonManagerScript::CButtonManagerScript()
    : CScript(UIMANAGERSCRIPT)
    , m_vBtn{}
    , m_iCurBtn(0)
    , m_iPrevBtn(0)
    , m_eDownKey(KEY::UP)
    , m_eUpKey(KEY::DOWN)
    , m_eSelectKey(KEY::SPACE)
{
}

CButtonManagerScript::~CButtonManagerScript()
{
}

void CButtonManagerScript::begin()
{
    if (m_vBtn.size() != 0)
        return;

    CLevel* _pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    vector<CGameObject*> _vBtnObject = _pCurLevel->GetLayer(15)->GetParentObjects();

    vector<CGameObject*>::iterator iter = _vBtnObject.begin();

    for (; iter != _vBtnObject.end(); iter++)
    {
        CButtonScript* _script = (*iter)->GetScript<CButtonScript>();

        if (nullptr != _script)
        {
            m_vBtn.push_back(_script);
        }
    }
}

void CButtonManagerScript::tick()
{
    if (m_vBtn.empty())
        return;

    ButtonManipulation();

    if (m_iPrevBtn != m_iCurBtn)
    {
        m_vBtn[m_iPrevBtn]->ChangeState(ButtonState::NORMAL);
        m_vBtn[m_iCurBtn]->ChangeState(ButtonState::SELECTED);

        m_iPrevBtn = m_iCurBtn;
    }

    if (m_vBtn[m_iCurBtn]->GetState() != ButtonState::DISABLED && (KEY_TAP(m_eSelectKey) || (KEY_TAP(LBTN) && m_vBtn[m_iCurBtn]->IsHovered())))
    {
        m_vBtn[m_iCurBtn]->Func();
    }

    if (m_vStageBtn.empty())
        return;
}

void CButtonManagerScript::ButtonManipulation()
{
    if (KEY_TAP(m_eDownKey))
    {
        if (m_iCurBtn != 0)
        {
            m_iCurBtn--;
        }
    }

    if (KEY_TAP(m_eUpKey))
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

UINT CButtonManagerScript::SaveToLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}

UINT CButtonManagerScript::LoadFromLevelFile(FILE* _File)
{
    UINT MemoryByte = 0;

    return MemoryByte;
}
