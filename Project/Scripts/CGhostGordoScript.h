#pragma once
#include "CMonsterUnitScript.h"

enum class GhostGordoState
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
    ReturnRotating,
    EyeCloseStart,
    End,
};

class CGhostGordoScript : public CMonsterUnitScript
{
private:
    GhostGordoState m_eState;
    Quat m_qBaseQuat;
    Vec3 m_vBasePos;
    float m_fAccTime;

public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

private:
    void InitSetting();
    void EnterState(GhostGordoState _state);
    void FSM();
    void ExitState(GhostGordoState _state);
    void ChangeState(GhostGordoState _state);
    void Move(Vec3 _vTarget);

private:
    void OnTriggerEnter(CCollider* _OtherCollider);
 
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
    void ReturnRotating();

public:
    CLONE(CGhostGordoScript)
    CGhostGordoScript();
    CGhostGordoScript(const CGhostGordoScript& Origin);
    virtual ~CGhostGordoScript();
};
