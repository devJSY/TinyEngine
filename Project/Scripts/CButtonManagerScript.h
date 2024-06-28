#pragma once
#include <Engine/CScript.h>

class CButtonScript;
class CButtonManagerScript : public CScript
{
private:
    vector<CButtonScript*> m_vBtn;

    UINT m_iCurBtn;
    UINT m_iPrevBtn;
public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ButtonManipulation();

public:
    virtual void SaveToLevelFile(FILE* _File) override{};
    virtual void LoadFromLevelFile(FILE* _File) override{};

    CLONE_DISABLE(CButtonManagerScript)
    
public:
    CButtonManagerScript();
    virtual ~CButtonManagerScript();
};
