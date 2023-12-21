#pragma once
#include "CEntity.h"

class CEditor : public CEntity
{
private:
    EDITOR_TYPE m_EditorType;

public:
    virtual void begin() = 0;
    virtual void tick() = 0;
    virtual void finaltick() = 0;
    virtual void render() = 0;

    virtual void Resize(){};

public:
    CEditor(EDITOR_TYPE type);
    virtual ~CEditor();
};
