#pragma once
#include "CComponent.h"

class CLight3D : public CComponent
{
private:
    LIGHT_TYPE m_Type;
    tLightData m_LightData;

public:
    LIGHT_TYPE GetLightType() const { return m_Type; }
    void SetLightType(LIGHT_TYPE type) { m_Type = type; }

    float GetFallOffStart() const { return m_LightData.fallOffStart; }
    void SetFallOffStart(float start) { m_LightData.fallOffStart = start; }

    float GetFallOffEnd() const { return m_LightData.fallOffEnd; }
    void SetFallOffEnd(float end) { m_LightData.fallOffEnd = end; }

    float GetSpotPower() const { return m_LightData.spotPower; }
    void SetSpotPower(float power) { m_LightData.spotPower = power; }

    Vec3 GetStrength() const { return m_LightData.strength; }
    void SetStrength(Vec3 strength) { m_LightData.strength = strength; }

public:
    virtual void finaltick() override;

public:
    CLight3D(LIGHT_TYPE type);
    virtual ~CLight3D();
};
