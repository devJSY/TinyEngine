#pragma once
#include "CCinematicScript.h"

class CHallofEternity_Floor1_CinematicScript : public CCinematicScript
{
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void OnTriggerEnter(CCollider2D* _OtherCollider) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CHallofEternity_Floor1_CinematicScript);

public:
    CHallofEternity_Floor1_CinematicScript();
    virtual ~CHallofEternity_Floor1_CinematicScript();
};
