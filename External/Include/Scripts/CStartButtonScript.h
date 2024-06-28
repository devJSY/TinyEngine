#pragma once
#include "ButtonScript.h"
class CStartButtonScript 
    : public ButtonScript
{
private:
    CGameObject* m_pTitleLogo;
    CGameObject* m_pNinLogo;
    CGameObject* m_pHalLogo; 

    CGameObject* m_pStarUI1;
    CGameObject* m_pStarUI2;

    CGameObject* m_pFileButton1;
    CGameObject* m_pFileButton2;
    CGameObject* m_pFileButton3;

public:
    virtual void SaveToLevelFile(FILE* _File){};
    virtual void LoadFromLevelFile(FILE* _File) {}
    
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CStartButtonScript)
    CStartButtonScript();
    virtual ~CStartButtonScript();
};
