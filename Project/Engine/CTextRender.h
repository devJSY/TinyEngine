#pragma once
#include "CRenderComponent.h"

class CTextRender : public CRenderComponent
{
private:
    wstring m_Text;
    float m_Size;
    Vec4 m_Color;
    int m_CameraIdx;

public:
    virtual void finaltick() override;
    virtual void UpdateData() override{};
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;
    virtual void render(UINT _Subset) override;

public:
    const wstring& GetText() const { return m_Text; }
    void SetText(const wstring& _Text) { m_Text = _Text; }

    float GetTextSize() const { return m_Size; }
    void SetTextSize(float _size) { m_Size = _size; }

    Vec4 GetTextColor() const { return m_Color; }
    void SetTextColor(Vec4 _color) { m_Color = _color; }

    int GetCameraIdx() const { return m_CameraIdx; }
    void SetCameraIdx(int _Idx) { m_CameraIdx = _Idx; }

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CTextRender);

public:
    CTextRender();
    virtual ~CTextRender();
};
