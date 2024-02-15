#pragma once
#include "CComponent.h"

class CLight3D : public CComponent
{
private:
    tLightInfo m_Info;
    Ptr<CTexture> m_DepthMapTex;
    int m_ShadowIdx; // Dynamic Light의 경우 그림자가 적용되는 광원의 번호 

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

    Ptr<CTexture> GetDepthMapTex() const { return m_DepthMapTex; }

    int GetShadowType() const { return m_Info.ShadowType; }
    void SetShadowType(int _type) { m_Info.ShadowType = _type; }
    
    int GetShadowIdx() const { return m_ShadowIdx; }
    void SetShadowIdx(int _Idx) { m_ShadowIdx = _Idx; } 

private:
    void CreateDepthMapTex();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CLight3D();
    virtual ~CLight3D();
};
