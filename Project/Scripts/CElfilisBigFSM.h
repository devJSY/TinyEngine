#pragma once
#include "CFSMScript.h"

class CElfilisBigFSM : public CFSMScript
{
private:
    UINT m_ComboLevel;
    vector<Ptr<CMaterial>> m_vecMtrls;
    wstring m_ReverseState;
    float m_PositionOffset;

public:
    virtual void begin() override;

    void Activate();
    void ReverseState(const wstring& _State) { m_ReverseState = _State; }
    virtual void ChangeState(const wstring& _strStateName) override;

public:
    void ClearComboLevel() { m_ComboLevel = 0; }
    void AddComboLevel() { m_ComboLevel++; }
    void SetComboLevel(UINT _Level) { m_ComboLevel = _Level; }
    void SetMtrlTransparent(float _Alpha);

    UINT GetComboLevel() { return m_ComboLevel; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CElfilisBigFSM)
    CElfilisBigFSM();
    CElfilisBigFSM(const CElfilisBigFSM& _Origin);
    virtual ~CElfilisBigFSM();
};