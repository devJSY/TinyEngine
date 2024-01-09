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

    void SetLightColor(Vec3 _vColor) { m_Info.vColor = _vColor; }
    void SetSpecular(Vec3 _vSpec) { m_Info.vSpecular = _vSpec; }
    void SetAmbient(Vec3 _vAmb) { m_Info.vAmbient = _vAmb; }

    Vec4 GetLightColor() const { return m_Info.vColor; }
    Vec4 GetSpecular() const { return m_Info.vSpecular; }
    Vec4 GetAmbient() const { return m_Info.vAmbient; }

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
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLight2D();
    virtual ~CLight2D();
};
