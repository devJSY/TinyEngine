#pragma once
#include "CRenderComponent.h"

class CMeshRender : public CRenderComponent
{
private:
    bool m_bMotionBlur;

public:
    virtual void finaltick() override;
    virtual void UpdateData() override{};
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;
    virtual void render(UINT _Subset) override;

public:
    bool IsUseMotionBlur() const { return m_bMotionBlur; }
    void SetUseMotionBlur(bool _Use) { m_bMotionBlur = _Use; }

public:
    CLONE(CMeshRender);

    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

public:
    CMeshRender();
    CMeshRender(const CMeshRender& origin);
    virtual ~CMeshRender();
};
