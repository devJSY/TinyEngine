#pragma once

#include "CButtonScript.h"

class CStageChangeButtonScript 
    : public CButtonScript
{
private:
    CGameObject* m_pManager;

    UINT m_iCurStage;
    UINT m_iStageCount;
    vector<string> m_vStageName;
    
    bool m_bClickedButton;

public:
    void SetStage(const UINT _iCount) { m_iCurStage = _iCount; }
    virtual void Func() override;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CStageChangeButtonScript)
public:
    CStageChangeButtonScript();
    virtual ~CStageChangeButtonScript();
};
