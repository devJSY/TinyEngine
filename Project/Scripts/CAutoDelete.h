#pragma once

#include <Engine/CScript.h>

class CAutoDelete : public CScript
{
private:
    float m_Duration;
    float m_Acc;

public:
    virtual void begin() override;
    virtual void tick() override;


public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CLONE(CAutoDelete);
    CAutoDelete();
    CAutoDelete(const CAutoDelete& _Origin);
    virtual ~CAutoDelete();

};
