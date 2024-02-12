#pragma once
#include "CRenderComponent.h"

enum class SKYBOX_TYPE
{
    SPHERE,
    BOX,
};

class CSkyBox : public CRenderComponent
{
private:
    SKYBOX_TYPE m_Type;
    Ptr<CTexture> m_BrdfTex;
    Ptr<CTexture> m_EnvTex;
    Ptr<CTexture> m_DiffuseTex;
    Ptr<CTexture> m_SpecularTex;

public:
    void SetType(SKYBOX_TYPE _Type);
    void SetBrdfTexture(Ptr<CTexture> _tex) { m_BrdfTex = _tex; }
    void SetEnvTexture(Ptr<CTexture> _tex) { m_EnvTex = _tex; }
    void SetDiffuseTexture(Ptr<CTexture> _tex) { m_DiffuseTex = _tex; }
    void SetSpecularTexture(Ptr<CTexture> _tex) { m_SpecularTex = _tex; }

public:
    virtual void begin() override;
    virtual void finaltick() override{};
    virtual void UpdateData() override{};
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    CSkyBox();
    virtual ~CSkyBox();
};
