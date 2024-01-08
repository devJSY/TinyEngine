#pragma once
#include "CComponent.h"

class CLight3D : public CComponent
{
private:
    LIGHT_TYPE  m_Type;
    tLightData  m_LightData;
    int         m_Idx;

public:
    LIGHT_TYPE GetLightType() const { return m_Type; }
    void SetLightType(LIGHT_TYPE type) { m_Type = type; }

    float GetFallOffStart() const { return m_LightData.fallOffStart; }
    void SetFallOffStart(float start) { m_LightData.fallOffStart = start; }

    float GetFallOffEnd() const { return m_LightData.fallOffEnd; }
    void SetFallOffEnd(float end) { m_LightData.fallOffEnd = end; }

    float GetSpotPower() const { return m_LightData.spotPower; }
    void SetSpotPower(float power) { m_LightData.spotPower = power; }

    Vec3 GetColor() const { return m_LightData.color; }
    void SetColor(Vec3 color) { m_LightData.color = color; }

public:
    virtual void finaltick() override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLight3D(LIGHT_TYPE type, int idx);
    virtual ~CLight3D();
};
