#pragma once
#include "CEditor.h"

class CSoundEditor : public CEditor
{
private:
    Ptr<CSound> m_Sound;
    float m_PlayPosition;
    float m_Volume;

public:
    virtual void init() override{};
    virtual void tick() override{};
    virtual void finaltick() override{};
    virtual void render(bool* open) override;

private:
    void render();

private:
    void DrawDetails();

public:
    void SetSound(Ptr<CSound> _Sound) { m_Sound = _Sound; }

public:
    CSoundEditor();
    virtual ~CSoundEditor();
};
