#pragma once
#include <Engine/CScript.h>

class CUITexScript;
class CStageChangeButtonScript;
class CUITexManagerScript : public CScript
{
private:
    UINT m_iLevel;
    float m_fDragDistance;
    UINT m_iPrevTex;
    UINT m_iCurTex;

    Vec2 m_vPrevMousePos;
    Vec2 m_vCurMousePos;

    vector<CUITexScript*> m_vUITexObject;
    CStageChangeButtonScript* m_vBtnObject;

    KEY m_eDownKey;
    KEY m_eUpKey;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void TexObjectManipulation();
    int Drag();

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE_DISABLE(CUITexManagerScript)
public:
    CUITexManagerScript();
    virtual ~CUITexManagerScript();
};
