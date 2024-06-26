#pragma once
#include "CRenderComponent.h"

enum class SKYBOX_SHAPE
{
    SPHERE,
    BOX,
};

class CSkyBox : public CRenderComponent
{
private:
    SKYBOX_SHAPE m_Shape;

    Ptr<CTexture> m_BrdfTex;
    Ptr<CTexture> m_EnvTex;
    Ptr<CTexture> m_DiffuseTex;
    Ptr<CTexture> m_SpecularTex;

public:
    SKYBOX_SHAPE GetSkyBoxShape() const { return m_Shape; };
    void SetShape(SKYBOX_SHAPE _shape);

    Ptr<CTexture> GetBrdfTex() const { return m_BrdfTex; }
    void SetBrdfTex(Ptr<CTexture> _BrdfTex) { m_BrdfTex = _BrdfTex; }

    Ptr<CTexture> GetEnvTex() const { return m_EnvTex; }
    void SetEnvTex(Ptr<CTexture> _EnvTex) { m_EnvTex = _EnvTex; }

    Ptr<CTexture> GetDiffuseTex() const { return m_DiffuseTex; }
    void SetDiffuseTex(Ptr<CTexture> _DiffuseTex) { m_DiffuseTex = _DiffuseTex; }

    Ptr<CTexture> GetSpecularTex() const { return m_SpecularTex; }
    void SetSpecularTex(Ptr<CTexture> _SpecularTex) { m_SpecularTex = _SpecularTex; }

public:
    virtual void finaltick() override;
    virtual void UpdateData();
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;
    virtual void render(UINT _Subset) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CSkyBox);

public:
    CSkyBox();
    virtual ~CSkyBox();
};
