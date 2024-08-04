#pragma once
#include <Engine\CScript.h>

enum class WipeState
{
    Wait,
    Minimize,
    Maximize,
    End,
};

class CUIWipeScript : public CScript
{
private:
    CGameObject* m_pPlayer;
    Vec3 m_vOriginScale;
    WipeState m_eState;
    float m_fTime;
    float m_fAccTime;
    float m_fRotSpeed;
    bool m_bRotate;

public:
    virtual void begin() override;
    virtual void tick() override;

    void ChangeState(WipeState _state);

private:
    void Tracking();
    void Rotate();

private:
    void EnterState(WipeState _state);
    void ExitState(WipeState _state);

private:
    void Wait();
    void Maximize();
    void Minimize();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIWipeScript)

public:
    CUIWipeScript();
    CUIWipeScript(const CUIWipeScript& Origin);
    virtual ~CUIWipeScript();
};
