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
    SurpriseBoardState m_eState;
    bool m_bIsRight;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void ChangeState(SurpriseBoardState _state);
    void EnterState(SurpriseBoardState _state);
    void ExitState(SurpriseBoardState _state);

private:
    void ArmMove();
    void PopOut();
    void PrePopOut();
    void Return();

private:
    virtual void OnTriggerEnter(CCollider* _OtherCollider);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CSurpriseBoardScript)
public:
    CSurpriseBoardScript();
    CSurpriseBoardScript(const CSurpriseBoardScript& Origin);
    virtual ~CSurpriseBoardScript();
};
