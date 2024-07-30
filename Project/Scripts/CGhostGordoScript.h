#pragma once
#include "CMonsterUnitScript.h"

enum class GHOSTGORDO_STATE
{
    EyeCloseWait,
    EyeOpenWait,
    OpenEye,
    Find,
    Track,
    TrackAfter1,
    TrackAfter2,
    TrackWait,
    Return,
    EyeCloseStart,
    End,
};

class CGhostGordoScript : public CMonsterUnitScript
{
private:
    GHOSTGORDO_STATE m_eState;
    Vec3 m_vBasePos;
    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void ChangeState(GHOSTGORDO_STATE _state);
    void EnterState(GHOSTGORDO_STATE _state);
    void ExitState(GHOSTGORDO_STATE _state);

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
    void OnTriggerExit(CCollider* _OtherCollider);

private:
    void EyeCloseWait();
    void EyeOpenWait();
    void OpenEye();
    void Find();
    void Track();
    void TrackAfter1();
    void TrackAfter2();
    void EyeCloseStart();
    void TrackWait();
    void Return();

public:
    CLONE(CGhostGordoScript)
    CGhostGordoScript();
    CGhostGordoScript(const CGhostGordoScript& _Origin);
    virtual ~CGhostGordoScript();
};
