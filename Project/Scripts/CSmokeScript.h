#pragma once

#include <Engine/CScript.h>

class CSmokeScript : public CScript
{
private:
    bool            m_bRandom;
    bool            m_bDirRandom;
    bool            m_bRotRandom;
    bool            m_bHorizontalDir;

    // 랜덤으로 생성할 경우 최대 Max값을 가진다.
    float           m_MinLifeTime;
    float           m_MaxLifeTime;
    float           m_MinSpeed;
    float           m_MaxSpeed;
    float           m_MinScale;
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

