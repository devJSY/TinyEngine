#pragma once
#include "CGameObject.h"

class CGameObjectEx : public CGameObject
{
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void finaltick() override;

    CLONE(CGameObjectEx);
};
