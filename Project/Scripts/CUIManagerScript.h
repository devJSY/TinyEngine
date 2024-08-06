#pragma once

#include <Engine/CScript.h>

enum class SceneType
{
    StartScene,
    RobbySccene,
    PlayScene,
    End,
};

class CButtonScript;
class CUIManagerScript : public CScript
{
private:
    map<wstring, CScript*> m_vUIManagerScript;

protected:
    SceneType m_eSceneState;

public:
    virtual void begin() override;
    virtual void tick() override;

private:
    void UIManagerScriptInit();

public:
    virtual UINT SaveToLevelFile(FILE* _File) override;
    virtual UINT LoadFromLevelFile(FILE* _File) override;

    CLONE_DISABLE(CUIManagerScript)
public:
    CUIManagerScript();
    virtual ~CUIManagerScript();
};
