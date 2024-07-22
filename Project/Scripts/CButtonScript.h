#pragma once
#include <Engine/CScript.h>

enum class ButtonState
{
    NORMAL,
    HIGHLIGHTED,
    PRESSED,
    SELECTED,
    DISABLED,
    END,
};

enum class ButtonTransition
{
    COLOR,
    TEXTURE,
    SCALE,
    ANIMATION,
    END,
};

class CButtonScript : public CScript
{
private:
    ButtonState m_ePrevState;
    ButtonState m_eCurState;

    ButtonTransition m_eTransition;

    bool m_IsInteraction;
    bool m_IsHovered;
    bool m_IsIsolated;

    Vec3 m_vNormalScale;

    // protected:
    //     std::function<void>* m_pFuncPtr;

private:
    Ptr<CTexture> m_pButtonTex[(UINT)ButtonState::END];
    Vec4 m_vButtonColor[(UINT)ButtonState::END];
    Vec2 m_vButtonScale[(UINT)ButtonState::END];

public:
    void ChangeState(ButtonState _eState)
    {
        m_ePrevState = m_eCurState;
        m_eCurState = _eState;
    }
    void SetTransition(const ButtonTransition _eTransition) { m_eTransition = _eTransition; }

    Ptr<CTexture> GetTexture(ButtonState _eState) const { return m_pButtonTex[(UINT)_eState]; }
    void SetTexture(const ButtonState _eState, Ptr<CTexture> _pTex) { m_pButtonTex[(UINT)_eState] = _pTex; }

    Vec4 GetColor(ButtonState _eState) const { return m_vButtonColor[(UINT)_eState]; }
    void SetColor(const ButtonState _eState, const Vec4 _vColor) { m_vButtonColor[(UINT)_eState] = _vColor; }

    Vec2 GetScale(ButtonState _eState) const { return m_vButtonScale[(UINT)_eState]; }
    void SetScale(const ButtonState _eState, const Vec2 _vScale) { m_vButtonScale[(UINT)_eState] = _vScale; }

    void SetIsolated(const bool _bFlag) { m_IsIsolated = _bFlag; }
    void SetInteraction(const bool _bFlag) { m_IsInteraction = _bFlag; }

    bool IsHovered() const { return m_IsHovered; }
    bool IsInteraction() const { return m_IsInteraction; }

    virtual void Func(){};

    ButtonState GetState() const { return m_eCurState; }

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void ButtonUpdate();

private:
    bool IsMouseHovered();
    bool IsRectInPoint(Vec2 _vWolrdMousePos);

public:
    CLONE(CButtonScript)

public:
    CButtonScript();
    CButtonScript(UINT _iScriptType);
    virtual ~CButtonScript();
};
