#pragma once
#include "CState.h"

class CElfilisA_StabCombo : public CState
{
public:
    virtual void tick() override;
    virtual void Enter() override;
    virtual void Exit() override;

public:
    CLONE(CElfilisA_StabCombo)
    CElfilisA_StabCombo();
    virtual ~CElfilisA_StabCombo();
};