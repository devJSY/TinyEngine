#pragma once
#include "CRenderComponent.h"

class CDecal : public CRenderComponent
{
private:
    Ptr<CTexture> m_DecalAlbedoTex;
    Ptr<CTexture> m_DecalMRATex;
    Ptr<CTexture> m_DecalNormalTex;
    Ptr<CTexture> m_DecalEmissiveTex;
    int m_bInvertNormalY;

public:
    Ptr<CTexture> GetDecalAlbedoTex() const { return m_DecalAlbedoTex; }
    void SetDecalAlbedoTex(Ptr<CTexture> _Tex) { m_DecalAlbedoTex = _Tex; }

    Ptr<CTexture> GetDecalMRATex() const { return m_DecalMRATex; }
    void SetDecalMRATex(Ptr<CTexture> _Tex) { m_DecalMRATex = _Tex; }

    Ptr<CTexture> GetDecalNormalTex() const { return m_DecalNormalTex; }
    void SetDecalNormalTex(Ptr<CTexture> _Tex) { m_DecalNormalTex = _Tex; }

    Ptr<CTexture> GetDecalEmissiveTex() const { return m_DecalEmissiveTex; }
    void SetDecalEmissiveTex(Ptr<CTexture> _Tex) { m_DecalEmissiveTex = _Tex; }

    bool IsInvertNormalY() const { return m_bInvertNormalY; }
    void SetInvertNormalY(bool _bInvertY) { m_bInvertNormalY = _bInvertY; }

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;
    virtual void render(UINT _Subset) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CDecal);

public:
    CDecal();
    virtual ~CDecal();
};
