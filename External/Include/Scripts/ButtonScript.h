#pragma once

#include <Engine/CScript.h>

enum class ButtonState
{
    NORMAL,
    HIGHLIGHT,
    PRESSED,
    SELECTED,
    DISABLED,
    END,
};

enum class ButtonTransition
{
    COLOR,
    TEXTURE,
    ANIMATION,
    END,
};

class ButtonScript : public CScript
{
protected:
    ButtonState m_ePrevState;
    ButtonState m_eCurState;
    ButtonTransition m_eTransition;
    bool m_bInteractable;

    std::function<void(int)> m_pFunction;

public:
    virtual void SaveToLevelFile(FILE* _File){};
    virtual void LoadFromLevelFile(FILE* _File){};

public:
    void SetTransition(ButtonTransition _trans) { m_eTransition = _trans; }

    void ChangeState(ButtonState _state)
    {
        m_ePrevState = m_eCurState;
        m_eCurState = _state;
    }

    void Interactable(bool _bInteractable) { m_bInteractable = _bInteractable; }

    bool Interactable() { return m_bInteractable; }

public:
    CLONE(ButtonScript)
    ButtonScript();
    ButtonScript(SCRIPT_TYPE _type);
    virtual ~ButtonScript();
};
