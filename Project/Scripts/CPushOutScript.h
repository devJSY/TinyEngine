#pragma once
#include <Engine\CScript.h>

enum class PushOutState
{
    MoveDest,
    MoveBase,
    Stop,
    End,
};

class CPushOutScript : public CScript
{

private:
    CGameObject* m_pChild[3];
    CGameObject* m_pPlayer;
    Vec3 m_vBase;
    Vec3 m_vDest;
    Vec3 m_vDir;
    int m_iChildNum[3];
    PushOutState m_eState;
    float m_fSpeed;
    float m_fReturnSpeed;
    float m_fMoveBaseOffset;
    float m_fMoveDestOffset;

    bool m_bXMove;
    bool m_bZMove;
    bool m_bBasePlus;
    bool m_bDestPlus;

public:
    virtual void begin() override;
    virtual void tick() override;
    
    void ChangeState(PushOutState _state);
    void EnterState();
    void ExitState(){};

public:
    void SetPlayer(CGameObject* _pObj) { m_pPlayer = _pObj; }

private:
    void MoveBase();
    void MoveDest();

    void Stop();
    void CheckPlayer();

private:
    void MovePlusX(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag);
    void MovePlusZ(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag);

    void MoveMinusX(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag);
    void MoveMinusZ(Vec3 _vDir, Vec3 _vDest, float _fSpeed, bool _flag);

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CPushOutScript)
public:
    CPushOutScript();
    CPushOutScript(const CPushOutScript& Origin);
    virtual ~CPushOutScript();
};
