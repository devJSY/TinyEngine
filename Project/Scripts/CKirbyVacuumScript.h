#pragma once

#include <Engine/CScript.h>

class CKirbyVacuumScript : public CScript
{
private:
    CGameObject* m_InsideVacuum;
    CGameObject* m_OutsdieVacuum;

    float m_Acc;

    float m_ScalingDuration;

    Vec4 m_StartColor;
    Vec4 m_EndColor;
    Vec4 m_InitColor;

    float m_StartGradationAlpha;
    float m_EndGradationAlpha;
    
    float m_RotationSpeed;
    float m_Frequency;


public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void Reset();


public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;


public:
    CLONE(CKirbyVacuumScript)
    CKirbyVacuumScript();
    CKirbyVacuumScript(const CKirbyVacuumScript& _Origin);
    virtual ~CKirbyVacuumScript();

};
