#pragma once
#include <Engine/CScript.h>
#include "CUITexManagerScript.h"

class CUITexScript 
    : public CScript
{
private:
    Ptr<CMaterial> m_pMtrl;
    vector<string> m_vTexName;

    UINT m_iPrevTex;
    UINT m_iCurTex;

    // 추 후 flow Manager 생길시에 levelCount로 추가
    UINT m_iCurTexCount;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetCurTexCount(UINT _iCount) { m_iCurTex = _iCount; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CUITexScript)
public:
    CUITexScript();
    virtual ~CUITexScript();
};
