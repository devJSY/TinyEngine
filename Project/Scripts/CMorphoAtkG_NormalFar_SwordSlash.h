#pragma once
#include "CBossState.h"

class CMorphoAtkG_NormalFar_SwordSlash : public CBossState
{
private:
    Ptr<CPrefab> m_SwordSlashPref;
    CGameObject* m_SwordSlash[3];
    float m_SlashSpeed;
    bool m_bFrmEnter;

public:
    virtual void tick() override;
    virtual void Exit() override;

private:
    virtual void Enter_Step() override;
    virtual void Exit_Step() override;

    // tick
    void Start();
    void Combo1();
    void Combo2();
    void Combo3();
    void End();

    CGameObject* CreateSwordSlash(Vec3 _PosOffset, float _RotZ);

public:
    CLONE(CMorphoAtkG_NormalFar_SwordSlash)
    CMorphoAtkG_NormalFar_SwordSlash();
    virtual ~CMorphoAtkG_NormalFar_SwordSlash();
};