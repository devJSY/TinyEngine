#pragma once
#include "CRenderComponent.h"

class CDecal : public CRenderComponent
{
private:
    Ptr<CTexture> m_DecalColorTex;
    Ptr<CTexture> m_DecalNormalTex;
    int m_bAsEmissive;

public:
    Ptr<CTexture> GetDecalColorTex() const { return m_DecalColorTex; }
    void SetDecalColorTex(Ptr<CTexture> _Tex) { m_DecalColorTex = _Tex; }

    Ptr<CTexture> GetDecalNormalTex() const { return m_DecalNormalTex; }
    void SetDecalNormalTex(Ptr<CTexture> _Tex) { m_DecalNormalTex = _Tex; }

    bool IsDecalAsEmissive() const { return m_bAsEmissive; }
    void SetDecalAsEmissive(bool _bTrue) { m_bAsEmissive = _bTrue; }

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CDecal);

public:
    CDecal();
    virtual ~CDecal();
};
