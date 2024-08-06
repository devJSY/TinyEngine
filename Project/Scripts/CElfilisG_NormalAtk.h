#pragma once
#include "CState.h"

class CElfilisG_NormalAtk : public CState
{
private:
	virtual void tick() override;
    virtual void Enter() override;
	virtual void Exit() override;

public:
    CLONE(CElfilisG_NormalAtk);
    CElfilisG_NormalAtk();
    virtual ~CElfilisG_NormalAtk();
};