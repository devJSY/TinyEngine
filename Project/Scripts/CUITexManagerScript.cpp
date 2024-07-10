#include "pch.h"
#include "CUITexManagerScript.h"

#include "CUITexScript.h"
#include "CStageChangeButtonScript.h"

CUITexManagerScript::CUITexManagerScript()
    : CScript(UITEXMANAGERSCRIPT)
    , m_iLevel(7)
    , m_vUITexObject{}
    , m_iPrevTex(0)
    , m_iCurTex(0)
    , m_fDragDistance(100.f)
    , m_eDownKey(KEY::LEFT)
    , m_eUpKey(KEY::RIGHT)
{
}

CUITexManagerScript::~CUITexManagerScript()
{
}

void CUITexManagerScript::begin()
{
    if (m_vUITexObject.size() != 0)
        return;

    CLevel* _pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
    vector<CGameObject*> _vUITexObject = _pCurLevel->GetLayer(15)->GetParentObjects();

    vector<CGameObject*>::iterator iter = _vUITexObject.begin();

    for (; iter != _vUITexObject.end(); iter++)
    {
        CUITexScript* _script = (*iter)->GetScript<CUITexScript>();

        CStageChangeButtonScript* _btnScript = (*iter)->GetScript<CStageChangeButtonScript>();

        if (nullptr != _script)
        {
            m_vUITexObject.push_back(_script);
        }

        if (nullptr != _btnScript)
        {
            m_vBtnObject = _btnScript;
        }
    }
}

void CUITexManagerScript::tick()
{
    if (m_vUITexObject.size() == 0)
        return;

    TexObjectManipulation();

    if (m_iPrevTex != m_iCurTex)
    {
        for (size_t i = 0; i < m_vUITexObject.size(); i++)
        {
            m_vUITexObject[i]->SetCurTexCount(static_cast<UINT>(m_iCurTex));
        }

        if (nullptr != m_vBtnObject)
            m_vBtnObject->SetStage(m_iCurTex);

        m_iPrevTex = m_iCurTex;
    }
}

void CUITexManagerScript::TexObjectManipulation()
{
    int _iIdx = 0;

    if (KEY_TAP(m_eDownKey))
    {
        if (m_iCurTex != 0)
        {
            _iIdx = -1;
        }
    }

    if (KEY_TAP(m_eUpKey))
    {
        if (m_iCurTex != m_iLevel - 1)
        {
            _iIdx = 1;
        }
    }

    int _iDrag = 0;
    _iDrag = Drag();

    _iDrag == 0 ? _iIdx += 0 : _iIdx = _iDrag;

    m_iCurTex += _iIdx;
}

int CUITexManagerScript::Drag()
{
    int _iIdx = 0;

    if (KEY_TAP(LBTN))
    {
        m_vPrevMousePos = CKeyMgr::GetInst()->GetMousePos();
    }

    if (KEY_RELEASED(LBTN))
    {
        m_vCurMousePos = CKeyMgr::GetInst()->GetMousePos();

        Vec2 _vDrag = m_vCurMousePos - m_vPrevMousePos;

        if (_vDrag.x <= -1.f * m_fDragDistance)
        {
            if (m_iCurTex != m_iLevel - 1)
            {
                _iIdx = 1;
            }
        }

        if (_vDrag.x >= m_fDragDistance)
        {
            if (m_iCurTex != 0)
            {
                _iIdx = -1;
            }
        }
    }

    return _iIdx;
}
    