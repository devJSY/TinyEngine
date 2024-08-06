#pragma once
#include "CUIAnimScript.h"

class CUIGlowMoveScript : public CUIAnimScript
{
private:
    float m_fStartOff;
    float m_fDist;
    float m_fSpeed;

public:
    virtual void tick() override;

private:
    void MoveGlowRatio();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CUIGlowMoveScript)
public:
    CUIGlowMoveScript();
    CUIGlowMoveScript(const CUIGlowMoveScript& Origin);
    virtual ~CUIGlowMoveScript();
};
