#pragma once
#include "CEditor.h"

class CSoundEditor : public CEditor
{
private:
    Ptr<CSound> m_Sound;
    int m_PlayPosition;
    float m_Volume;
    float m_Pitch;

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
    void SetSound(Ptr<CSound> _Sound);

public:
    CSoundEditor();
    virtual ~CSoundEditor();
};
