#pragma once
#include "CRenderComponent.h"

class CLandScape : public CRenderComponent
{
private:
    UINT m_FaceX;
    UINT m_FaceZ;

    Ptr<CTexture> m_HeightMapTex;

public:
    void SetHeightMap(Ptr<CTexture> _HeightMap) { m_HeightMapTex = _HeightMap; }

private:
    void Init();
    void CreateMesh();

public:
    virtual void finaltick() override;
    virtual void UpdateData() override;
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CLandScape);

public:
    CLandScape();
    virtual ~CLandScape();
};
