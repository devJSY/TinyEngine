#pragma once
#include <Engine\CScript.h>

enum class HPState
{
    Enter,
    Tick,
    End,
};

class CUnitScript;
class CUIBossHPScript : public CScript
{
private:
    HPState m_eState;

    CGameObject* m_pNameObj;
    CGameObject* m_pNameObj2;
    string m_TargetName;

    CUnitScript* m_pUnitScript;

    CMeshRender* m_pRenderer;

    float m_fAccTime;
    float m_fComboTime;

    float m_fMaxHP;
    float m_fCurHP;
    float m_fPrevHP;

    float m_fEnterHP;

    bool m_bIsCombo;
    bool m_bIsScaling;

    Vec4 m_vDecreaseColor;
    Vec4 m_vBasicColor;

    float m_fDescSpeed;

    bool m_bMolPho;
    bool m_bElfilis;

public:
    virtual void begin() override;
    virtual void tick() override;

    void ChangeState(HPState _state);

private:
    void EnterState();

private:
    void Enter();
    void Tick();
    void End();

private:
    void CaculateShading();
    void Scaling();

private:
    bool IsCombo();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIBossHPScript)
public:
    CUIBossHPScript();
    CUIBossHPScript(const CUIBossHPScript& Origin);
    virtual ~CUIBossHPScript();
};
