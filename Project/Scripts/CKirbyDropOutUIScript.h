#pragma once
#include <Engine\CScript.h>

enum class DropOutUIState
{
    Wait,
    Idle,
    Appear,
    DropOut,
    Disappear,
    End,
};

class CKirbyDropOutUIScript : public CScript
{
private:
    vector<CGameObject*> m_pProgressUI;
    CGameObject* m_pCircleProgressUI;
    DropOutUIState m_eState;

    Vec3 m_vOffset;

    float m_fAccTime;
    float m_fAppearTime;
    float m_fDisappearTime;
    float m_fWaitTime;

    bool m_bFailed;
    bool m_bInterAppear;

public:
    void SetInteraction(const bool _bFlag) { m_bInterAppear = _bFlag; }

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Idle();
    void Wait();
    void Appear();
    void Disappear();

private:
    void ChangeState(DropOutUIState _eState);
    void EnterState();
    void ExitState();

private:
    void FadeFunc(bool _fadeIn);
    void TrackPlayer();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;
    CLONE(CKirbyDropOutUIScript)
public:
    CKirbyDropOutUIScript();
    CKirbyDropOutUIScript(const CKirbyDropOutUIScript& Origin);
    virtual ~CKirbyDropOutUIScript();
};
