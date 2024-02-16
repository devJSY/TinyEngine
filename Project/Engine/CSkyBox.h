#pragma once
#include "CRenderComponent.h"

enum class SKYBOX_TYPE
{
    IBLBaker,
    LearnOpenGL,
    moonless,
    PureSky,
    END
};

enum class SKYBOX_SHAPE
{
    SPHERE,
    BOX,
};

class CSkyBox : public CRenderComponent
{
private:
    SKYBOX_TYPE m_Type;
    SKYBOX_SHAPE m_Shape;

    Ptr<CTexture> m_BrdfTex;
    Ptr<CTexture> m_EnvTex;
    Ptr<CTexture> m_DiffuseTex;
    Ptr<CTexture> m_SpecularTex;

public:
    void SetType(SKYBOX_TYPE _type);
    void SetShape(SKYBOX_SHAPE _shape);

    SKYBOX_TYPE GetSkyBoxType() const { return m_Type; };
    SKYBOX_SHAPE GetSkyBoxShape() const { return m_Shape; };

public:
    virtual void begin() override;
    virtual void finaltick() override;
    virtual void UpdateData();
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CSkyBox);

public:
    CSkyBox();
    virtual ~CSkyBox();
};
