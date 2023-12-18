#pragma once
#include "CScript.h"

class CPlayerScript : public CScript
{
private:
    float m_Speed;

public:
    virtual void tick() override;

public:
    CPlayerScript();
    virtual ~CPlayerScript();
};
