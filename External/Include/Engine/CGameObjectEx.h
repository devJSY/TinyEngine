#pragma once
#include "CGameObject.h"

class CGameObjectEx : public CGameObject
{
public:
    virtual void finaltick() override;

    CLONE(CGameObjectEx);
};
