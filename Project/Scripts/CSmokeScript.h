#pragma once

#include <Engine/CScript.h>

class CSmokeScript : public CScript
{
private:
    bool            m_bRandom;

    // �������� ������ ��� �ִ� Max���� ������.
    float           m_MaxLifeTime;
    float           m_MaxSpeed;
    float           m_MaxScale;

    float           m_Acc;
    float           m_LifeTime;
    float           m_Speed;
    float           m_Scale;
    Vec3            m_Dir;

    float           m_FadeOutAlpha;

private:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;


public:
    CLONE(CSmokeScript);
    CSmokeScript();
    CSmokeScript(const CSmokeScript& _Origin);
    virtual ~CSmokeScript();
};
