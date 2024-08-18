#pragma once
#include <Engine/CScript.h>

class CUnitScript;
class CKirbyFSM;
class CUIHPScript : public CScript
{
private:
    CGameObject* m_pNameObj;
    string m_TargetName;

    CKirbyFSM* m_pFSMScript;
    CUnitScript* m_pUnitScript;

    CMeshRender* m_pRenderer;

    float m_fAccTime;
    float m_fComboTime;

    float m_fMaxHP;
    float m_fCurHP;
    float m_fPrevHP;

    bool m_bIsCombo;
    bool m_bIsScaling;

    Vec4 m_vDecreaseColor;
    Vec4 m_vBasicColor;

    float m_fDescSpeed;

    bool m_bIsEnter;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetPlayer();
    void SetInitInfo();

private:
    void CaculateShading();
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
