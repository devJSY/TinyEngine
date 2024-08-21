#pragma once
#include <Engine/CScript.h>

class CUnitScript;
class CKirbyFSM;

enum class UnitHPState
{
    Damaged,
    Healing,
    Wait,
    End,
};

class CUIHPScript : public CScript
{
private:
    UnitHPState m_eState;
    CGameObject* m_pNameObj;
    string m_TargetName;

    CKirbyFSM* m_pFSMScript;
    CUnitScript* m_pUnitScript;

    CMeshRender* m_pRenderer;

    float m_fAccTime;
    float m_fComboTime;
    float m_fHealingTime;

    float m_fMaxHP;
    float m_fCurHP;
    float m_fCurPrevHP;
    float m_fPrevHP;

    bool m_bIsCombo;
    bool m_bIsScaling;

    Vec4 m_vDecreaseColor;
    Vec4 m_vBasicColor;

    float m_fDescSpeed;

    bool m_bIsEnter;
    bool m_bHpHealing;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(UnitHPState _eState);
    void EnterState();
    void ExitState();

private:
    void Damaged();
    void Healing();
    void Wait();

public:
    void SetPlayer();
    void SetInitInfo();

    void SetHealingTime(float _fValue) { m_fHealingTime = _fValue; }

private:
    void CaculateShading();
    void CaculateHealingShading();
    void Scaling();
    void SwitchKirbyName();

private:
    bool IsCombo();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIHPScript)
public:
    CUIHPScript();
    CUIHPScript(const CUIHPScript& Origin);
    virtual ~CUIHPScript();
};
