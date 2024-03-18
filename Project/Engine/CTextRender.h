#pragma once
#include "CRenderComponent.h"

class CTextRender : public CRenderComponent
{
private:
    wstring m_Text;
    Vec2 m_Position;
    float m_Size;
    Vec4 m_Color;

public:
    virtual void UpdateData() override{};
    virtual void render() override;
    virtual void render(Ptr<CMaterial> _mtrl) override;

public:
    const wstring& GetText() const { return m_Text; }
    void SetText(const wstring& _Text) { m_Text = _Text; }

    Vec2 GetTextPosition() const { return m_Position; }
    void SetTextPosition(Vec2 _pos) { m_Position = _pos; }

    float GetTextSize() const { return m_Size; }
    void SetTextSize(float _size) { m_Size = _size; }

    Vec4 GetTextColor() const { return m_Color; }
    void SetTextColor(Vec4 _color) { m_Color = _color; }

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CTextRender);

public:
    CTextRender();
    virtual ~CTextRender();
};
