#pragma once
#include "CComponent.h"

class CLight3D : public CComponent
{
private:
    LIGHT_TYPE m_Type;
    tLight m_Light;

    float m_FallOffStart;
    float m_FallOffEnd;
    float m_SpotPower;
    Vec3 m_Strength;

public:
    LIGHT_TYPE GetLightType() const { return m_Type; }
    void SetLightType(LIGHT_TYPE type) { m_Type = type; }

    float GetFallOffStart() const { return m_FallOffStart; }
    void SetFallOffStart(float start) { m_FallOffStart = start; }

    float GetFallOffEnd() const { return m_FallOffEnd; }
    void SetFallOffEnd(float end) { m_FallOffEnd = end; }

    float GetSpotPower() const { return m_SpotPower; }
    void SetSpotPower(float power) { m_SpotPower = power; }

    Vec3 GetStrength() const { return m_Strength; }
    void SetStrength(Vec3 strength) { m_Strength = strength; }

public:
    virtual void finaltick() override;

public:
    CLight3D(LIGHT_TYPE type);
    virtual ~CLight3D();
};
