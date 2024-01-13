#pragma once
#include "CEditor.h"

class CMaterialEditor : public CEditor
{
private:
    Ptr<CMaterial> m_Mtrl;

public:
    virtual void init(){};
    virtual void tick(){};
    virtual void finaltick(){};
    virtual void render();

public:
    CMaterialEditor(Ptr<CMaterial> pMtrl);
    virtual ~CMaterialEditor();
};
