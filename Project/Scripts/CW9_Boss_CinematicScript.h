#pragma once
#include "CCinematicScript.h"

class CW9_Boss_CinematicScript : public CCinematicScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;

public:
    CLONE(CW9_Boss_CinematicScript);

public:
    CW9_Boss_CinematicScript();
    virtual ~CW9_Boss_CinematicScript();
};
