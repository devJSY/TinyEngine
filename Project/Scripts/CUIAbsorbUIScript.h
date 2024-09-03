#pragma once
#include <Engine\CScript.h>

enum class AbsorbUIState
{
    Progress,
    End,
};

class CUIAbsorbUIScript : public CScript
{
private:
    CGameObject* m_pObj;
    Vec3 m_vOffset;
    AbsorbUIState m_eState;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void ChangeState(AbsorbUIState _eState) { m_eState = _eState; }
    void SetTarget(CGameObject* _pObj) { m_pObj = _pObj; }

private:
    void Progress();
    void End();

    void TargetTrack();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIAbsorbUIScript)
public:
    CUIAbsorbUIScript();
    CUIAbsorbUIScript(const CUIAbsorbUIScript& Origin);
    virtual ~CUIAbsorbUIScript();
};
