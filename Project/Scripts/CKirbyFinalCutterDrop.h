#pragma once
#include "CState.h"
class CKirbyFinalCutterDrop : public CState
{
private:
    float m_SaveGravity;

public:
    virtual void tick();
    virtual void Enter();
    virtual void Exit();

public:
    CLONE(CKirbyFinalCutterDrop)
    CKirbyFinalCutterDrop();
    virtual ~CKirbyFinalCutterDrop();
};
