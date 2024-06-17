#pragma once
#include "CComponent.h"

class CGameObjectEx;

class CLight : public CComponent
{
private:
    tLightInfo m_Info;
    Ptr<CTexture> m_DepthMapTex;

    CGameObjectEx* m_pLightCam; // 광원 시점용 카메라

    Ptr<CMesh> m_VolumeMesh;
    Ptr<CMaterial> m_LightMtrl;

public:
    virtual void finaltick() override;

public:
    const tLightInfo& GetLightInfo() const { return m_Info; }

    Vec4 GetLightRadiance() const { return m_Info.vRadiance; }
    void SetLightRadiance(Vec3 _vRadiance) { m_Info.vRadiance = _vRadiance; }

    float GetRadius() const { return m_Info.fRadius; }
    void SetRadius(float _Radius) { m_Info.fRadius = _Radius; }

    void SetAngle(float _Angle) { m_Info.fAngle = _Angle; }
    float GetAngle() const { return m_Info.fAngle; }

    LIGHT_TYPE GetLightType() const { return (LIGHT_TYPE)m_Info.LightType; }
    void SetLightType(LIGHT_TYPE _type);

    float GetFallOffStart() const { return m_Info.fallOffStart; }
    void SetFallOffStart(float start) { m_Info.fallOffStart = start; }

    float GetFallOffEnd() const { return m_Info.fallOffEnd; }
    void SetFallOffEnd(float end) { m_Info.fallOffEnd = end; }

    float GetSpotPower() const { return m_Info.spotPower; }
    void SetSpotPower(float power) { m_Info.spotPower = power; }

    int GetShadowIdx() const { return m_Info.ShadowIndex; }
    void SetShadowIdx(int _Idx) { m_Info.ShadowIndex = _Idx; }

    float GetHaloRadius() const { return m_Info.HaloRadius; }
    void SetHaloRadius(float _Radius) { m_Info.HaloRadius = _Radius; }

    float GetHaloStrength() const { return m_Info.HaloStrength; }
    void SetHaloStrength(float _Strength) { m_Info.HaloStrength = _Strength; }

    Ptr<CTexture> GetDepthMapTex() const { return m_DepthMapTex; }

public:
    void render_Deferred(int _LightIdx);
    void render_ShadowDepth(UINT _MobilityType);

private:
    void CreateDepthMapTex();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLight);

public:
    CLight();
    CLight(const CLight& origin);
    virtual ~CLight();
};
