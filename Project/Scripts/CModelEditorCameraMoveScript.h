#pragma once
#include <Engine/CScript.h>

class CModelEditorCameraMoveScript : public CScript
{
private:
    float m_CamSpeed;

public:
    virtual void tick();

private:
    void MovePerspective();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE(CModelEditorCameraMoveScript);

public:
    CModelEditorCameraMoveScript();
    CModelEditorCameraMoveScript(const CModelEditorCameraMoveScript& origin);
    virtual ~CModelEditorCameraMoveScript();
};