#pragma once
#include <Engine\CScript.h>

enum class DropOutUIState
{
    Wait,
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

    bool m_bFailed;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
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
