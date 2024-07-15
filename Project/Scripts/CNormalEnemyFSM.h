#pragma once
#include "CFSMScript.h"
class CNormalEnemyFSM 
	: public CFSMScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    CLONE(CNormalEnemyFSM)
	CNormalEnemyFSM();
    virtual ~CNormalEnemyFSM();
};
