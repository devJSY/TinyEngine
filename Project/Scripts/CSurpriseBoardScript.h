#pragma once
#include <Engine\CScript.h>

enum class SurpriseBoardState
{
    ArmMove,
    Break,
    PopOut,
    PrePopOut,
    Return,
    Wait,
    End,
};

class CSurpriseBoardScript : public CScript
{
private:
    CGameObject* m_pWallColObject;
    CGameObject* m_pAtkObject1;
    CGameObject* m_pAtkObject2;
    CGameObject* m_pPushOutObj;
    SurpriseBoardState m_eState;

    int m_iPushOutNum;
    bool m_bIsRight;
    bool m_bChainAction;

public:
    virtual void begin() override;
    virtual void tick() override;

    void SetChainAction(const bool _flag) { m_bChainAction = _flag; }

    void ChangeState(SurpriseBoardState _state);
    SurpriseBoardState GetState() { return m_eState; }

private:
    void EnterState(SurpriseBoardState _state);
    void ExitState(SurpriseBoardState _state);

private:
    void ArmMove();
    void PopOut();
    void PrePopOut();
    void Return();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CSurpriseBoardScript)
public:
    CSurpriseBoardScript();
    CSurpriseBoardScript(const CSurpriseBoardScript& Origin);
    virtual ~CSurpriseBoardScript();
};
