#pragma once
#include "CComponent.h"

class CSoundListener : public CComponent
{
public:
    virtual void finaltick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CSoundListener);

public:
    CSoundListener();
    virtual ~CSoundListener();
};
