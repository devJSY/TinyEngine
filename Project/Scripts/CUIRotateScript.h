#pragma once
#include "CUIAnimScript.h"

class CUIRotateScript : public CUIAnimScript
{
private:
    float m_fSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void Rotate();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIRotateScript)
public:
    CUIRotateScript();
    CUIRotateScript(const CUIRotateScript& Origin);
    virtual ~CUIRotateScript();
};
