#pragma once
#include "CComponent.h"

class CLight2D : public CComponent
{
private:
    tLightInfo m_Info;

public:
    virtual void finaltick() override;

public:
    const tLightInfo& GetLightInfo() const { return m_Info; }

    void SetLightRadiance(Vec3 _vRadiance) { m_Info.vRadiance = _vRadiance; }
    Vec4 GetLightRadiance() const { return m_Info.vRadiance; }

    void SetLightType(LIGHT_TYPE _type) { m_Info.LightType = (int)_type; }
    void SetRadius(float _Radius) { m_Info.fRadius = _Radius; }
    void SetAngle(float _Angle) { m_Info.fAngle = _Angle; }

    LIGHT_TYPE GetLightType() const { return (LIGHT_TYPE)m_Info.LightType; }
    float GetRadius() const { return m_Info.fRadius; }
    float GetAngle() const { return m_Info.fAngle; }

    float GetFallOffStart() const { return m_Info.fallOffStart; }
    void SetFallOffStart(float start) { m_Info.fallOffStart = start; }

    float GetFallOffEnd() const { return m_Info.fallOffEnd; }
    void SetFallOffEnd(float end) { m_Info.fallOffEnd = end; }

    float GetSpotPower() const { return m_Info.spotPower; }
    void SetSpotPower(float power) { m_Info.spotPower = power; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CLight2D);

public:
    CLight2D();
    virtual ~CLight2D();
};
